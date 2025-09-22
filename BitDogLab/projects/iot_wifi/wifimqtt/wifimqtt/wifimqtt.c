#include "main.h"
#define PUB_DELAY_MS 10000
static uint32_t counter = 0;
static uint32_t last_time = 0;
err_t mqtt_test_connect(MQTT_CLIENT_T *state);

void dns_found(const char *name, const ip_addr_t *ipaddr, void *callback_arg)
{
    MQTT_CLIENT_T *state = (MQTT_CLIENT_T *)callback_arg;
    if (ipaddr)
    {
        state->remote_addr = *ipaddr;
        DEBUG_printf("DNS resolved: %s\n", ip4addr_ntoa(ipaddr));
    }
    else
    {
        DEBUG_printf("DNS resolution failed.\n");
    }
}

void run_dns_lookup(MQTT_CLIENT_T *state)
{
    DEBUG_printf("Running DNS lookup for %s...\n", MQTT_SERVER_HOST);
    if (dns_gethostbyname(MQTT_SERVER_HOST, &(state->remote_addr), dns_found, state) == ERR_INPROGRESS)
    {
        while (state->remote_addr.addr == 0)
        {
            cyw43_arch_poll();
            sleep_ms(10);
        }
    }
}

static void mqtt_pub_start_cb(void *arg, const char *topic, u32_t tot_len)
{
    DEBUG_printf("Incoming message on topic: %s\n", topic);
}

static void mqtt_pub_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
    char buffer[BUFFER_SIZE];
    if (len < BUFFER_SIZE)
    {
        memcpy(buffer, data, len);
        buffer[len] = '\0';

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        for (int i = 0; i < len; i++)
        {
            if (buffer[i] == '\n' || buffer[i] == '\r')
            {
                buffer[i] = '\0';
                break;
            }
        }
        //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        DEBUG_printf("Message received: '%s'\n", buffer);

        if (strcmp(buffer, "acender") == 0)
        {
            pwm_led(LED_PIN_B, 3000);
        }
        else if (strcmp(buffer, "apagar") == 0)
        {
            pwm_led(LED_PIN_B, 0);
        }
    }
    else
    {
        DEBUG_printf("Message too large, discarding.\n");
    }
}

static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
    if (status == MQTT_CONNECT_ACCEPTED)
    {
        DEBUG_printf("MQTT connected.\n");
    }
    else
    {
        pwm_led(LED_PIN_R, 0);
        DEBUG_printf("MQTT connection failed: %d\n", status);
    }
}

void mqtt_pub_request_cb(void *arg, err_t err)
{
    DEBUG_printf("Publish request status: %d\n", err);
}

void mqtt_sub_request_cb(void *arg, err_t err)
{
    DEBUG_printf("Subscription request status: %d\n", err);
}

err_t mqtt_test_publish(MQTT_CLIENT_T *state)
{
    uint32_t now = to_ms_since_boot(get_absolute_time());
    if (now - last_time >= PUB_DELAY_MS)
    {
        
        char buffer[BUFFER_SIZE];
        printf("time: %d\n", (now - last_time));
        last_time = now;
        counter += 1;
        snprintf(buffer, BUFFER_SIZE, "MQTT_EMBARCATECH !");
        mqtt_publish(state->mqtt_client, "pico_w/topic1", buffer, strlen(buffer), 0, 0, mqtt_pub_request_cb, state);
        snprintf(buffer, BUFFER_SIZE, "\nMensagem numero: %u", counter);
        return mqtt_publish(state->mqtt_client, "pico_w/topic2", buffer, strlen(buffer), 0, 0, mqtt_pub_request_cb, state);
    }
    else
    {
        return 0;
    }
}

err_t mqtt_test_connect(MQTT_CLIENT_T *state)
{
    struct mqtt_connect_client_info_t ci = {0};
    ci.client_id = "PicoW";
    return mqtt_client_connect(state->mqtt_client, &(state->remote_addr), MQTT_SERVER_PORT, mqtt_connection_cb, state, &ci);
}



void mqtt_run_test(MQTT_CLIENT_T *state)
{
    state->mqtt_client = mqtt_client_new();
    if (!state->mqtt_client)
    {
        DEBUG_printf("Failed to create MQTT client\n");
        return;
    }

    if (mqtt_test_connect(state) == ERR_OK)
    {
        mqtt_set_inpub_callback(state->mqtt_client, mqtt_pub_start_cb, mqtt_pub_data_cb, NULL);
        mqtt_sub_unsub(state->mqtt_client, "pico_w/recv", 0, mqtt_sub_request_cb, NULL, 1);

        while (1)
        {
            cyw43_arch_poll();
            if (mqtt_client_is_connected(state->mqtt_client))
            {
                mqtt_test_publish(state);
                if (ledverdestatus == 1)
                {
                    pwm_led(LED_PIN_G, 0);
                }
                static uint64_t last_time_pwm = 0;
                uint64_t current_time = to_ms_since_boot(get_absolute_time());
                if (current_time - last_time_pwm >= FADE_STEP_DELAY) // a cada 100ms vai aumentar o brilho em 400
                {
                    last_time_pwm = current_time;
                }
                sleep_ms(50);
            }
            else
            {
                DEBUG_printf("Reconnecting...\n");
                sleep_ms(250);
                mqtt_test_connect(state);
            }
        }
    }
}