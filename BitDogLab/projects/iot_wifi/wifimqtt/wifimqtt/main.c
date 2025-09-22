#include "main.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/tcp.h"
#include "hardware/pwm.h" 
#include "hardware/adc.h"

#define TCP_SERVER_IP "192.168.1.23"
#define TCP_PORT 8000
#define DEBUG_printf printf

// Buffer para o JSON que será enviado
char g_json_payload[128];

// Estrutura para manter o estado da conexão
typedef struct TCP_CLIENT_T_ {
    struct tcp_pcb *tcp_pcb;
    ip_addr_t remote_addr;
} TCP_CLIENT_T;

// Variáveis para comunicação entre a interrupção e o loop principal
volatile bool g_button_pressed_flag = false;
volatile uint g_last_button_gpio = 0;

// Função para fechar a conexão TCP
static err_t tcp_client_close(void *arg) {
    TCP_CLIENT_T *state = (TCP_CLIENT_T*)arg;
    if (state->tcp_pcb != NULL) {
        tcp_close(state->tcp_pcb);
        state->tcp_pcb = NULL;
    }
    free(state); // Libera a memória alocada para o estado
    return ERR_OK;
}

// Callback para quando a conexão é estabelecida
static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err) {
    if (err != ERR_OK) {
        DEBUG_printf("Falha na conexão: %d\r\n", err);
        return tcp_client_close(arg);
    }
    DEBUG_printf("Conectado ao servidor. Enviando dados...\r\n");
    DEBUG_printf("Payload: %s\n", g_json_payload);

    // Envia o payload JSON que foi preparado no loop principal
    cyw43_arch_lwip_begin();
    err = tcp_write(tpcb, g_json_payload, strlen(g_json_payload), TCP_WRITE_FLAG_COPY);
    cyw43_arch_lwip_end();

    if (err != ERR_OK) {
        DEBUG_printf("Falha ao escrever no buffer TCP: %d\r\n", err);
    }
    
    return tcp_client_close(arg);
}

// Função principal para iniciar a conexão
static bool tcp_client_open(void) {
    TCP_CLIENT_T *state = calloc(1, sizeof(TCP_CLIENT_T));
    if (!state) {
        DEBUG_printf("Falha ao alocar estado\n");
        return false;
    }
    ip4addr_aton(TCP_SERVER_IP, &state->remote_addr);

    state->tcp_pcb = tcp_new_ip_type(IPADDR_TYPE_V4);
    if (!state->tcp_pcb) {
        DEBUG_printf("Falha ao criar pcb\n");
        free(state);
        return false;
    }

    tcp_arg(state->tcp_pcb, state);
    
    cyw43_arch_lwip_begin();
    err_t err = tcp_connect(state->tcp_pcb, &state->remote_addr, TCP_PORT, tcp_client_connected);
    cyw43_arch_lwip_end();

    return err == ERR_OK;
}

// Função para ler o sensor de temperatura interno
float read_onboard_temperature() {
    // Seleciona o canal 4 do ADC, que é o sensor de temperatura
    adc_select_input(4);

    // Lê o valor cru de 12 bits do ADC
    uint16_t raw = adc_read();

    // Converte o valor cru para tensão (a referência é 3.3V)
    const float conversion_factor = 3.3f / (1 << 12);
    float voltage = raw * conversion_factor;

    // Converte a tensão para graus Celsius usando a fórmula do datasheet
    float temp_c = 27.0f - (voltage - 0.706f) / 0.001721f;

    return temp_c;
}

// Callback de interrupção para os botões
void gpio_callback(uint gpio, uint32_t events) {
    // Implementa um debounce simples
    static uint32_t last_interrupt_time = 0;
    uint32_t now = to_ms_since_boot(get_absolute_time());
    if ((now - last_interrupt_time) > 250) {
        last_interrupt_time = now;
        g_last_button_gpio = gpio;      // Guarda qual pino foi pressionado
        g_button_pressed_flag = true;   // Avisa o loop principal que um botão foi pressionado
    }
}

// Função para controlar o LED RGB
void set_led_color(int r, int g, int b) {
    pwm_set_gpio_level(LED_PIN_R, r * r);
    pwm_set_gpio_level(LED_PIN_G, g * g);
    pwm_set_gpio_level(LED_PIN_B, b * b);
}

/*
* Finaliza as configurações TCP IP
*/

// static MQTT_CLIENT_T* mqtt_client_init(void) {
//     MQTT_CLIENT_T *state = calloc(1, sizeof(MQTT_CLIENT_T));
//     if (!state) {
//         DEBUG_printf("Failed to allocate state\r\n");
//         return NULL;
//     }
//     return state;
// }

int main() {
    stdio_init_all();
    pinos_start();

    //Habilita o sensor de temperatura
    adc_set_temp_sensor_enabled(true);

    // Habilita as interrupções para os botões
    gpio_set_irq_enabled_with_callback(BUTTON5_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(BUTTON6_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(BUTTONJS_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    printf("\n--> Iniciando conexao ao Wi-Fi <--\r\n");

    // 1. Inicia a conexao
    if (cyw43_arch_init()) {
        DEBUG_printf("Falha ao iniciar o Wi-Fi\r\n");
        return 1;
    }
    
    cyw43_arch_enable_sta_mode();
    printf("Conectando ao Wi-Fi: %s\r\n", WIFI_SSID);
    set_led_color(0, 0, 255); // Conecta aciona LED Azul

    // 2. Tente conectar APENAS UMA VEZ.
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        DEBUG_printf("Falha ao conectar com o Wi-Fi\r\n");
        set_led_color(255, 0, 0); // Falha conexao aciona LED Vermelho
        return 1;
    } 
    
    // 3. Se chegou aqui, conectou
    printf("Conectado ao Wi-Fi com sucesso!\r\n");
    set_led_color(0, 255, 0); // Conectado aciona LED Verde

    while (true) {
        // Verifica se a flag de botão pressionado foi acionada pela interrupção
        if (g_button_pressed_flag) {
            g_button_pressed_flag = false; // Reseta a flag

            char button_name[5];
            if (g_last_button_gpio == BUTTON5_PIN) {
                strcpy(button_name, "B5");
            } else if (g_last_button_gpio == BUTTON6_PIN) {
                strcpy(button_name, "B6");
            } else if (g_last_button_gpio == BUTTONJS_PIN) {
                strcpy(button_name, "BJS");
            } else {
                strcpy(button_name, "UNK"); // Desconhecido
            }

            // Lê a temperatura antes de enviar
            float current_temp = read_onboard_temperature();

            printf("Botão %s pressionado. Temp: %.2fC. Enviando...\n", button_name, current_temp);

            // Monta o payload JSON com a informação do botão <-- estudar mais
            sprintf(g_json_payload, "{\"button_id\": \"%s\", \"status\": \"pressionado\", \"temperature\": %.2f}", button_name, current_temp);

            set_led_color(255, 255, 0); // Enviar dados LED Amarelo/Laranja
            
            // Inicia o processo de envio
            tcp_client_open();

            sleep_ms(500); // Um pequeno delay para o LED
            set_led_color(0, 255, 0); // Conectado e ocioso LED Verde
        }
        
        sleep_ms(10); // Pequeno delay para não sobrecarregar o processador
    }

    // MQTT_CLIENT_T *state = mqtt_client_init();
    // run_dns_lookup(state);
    // mqtt_run_test(state);

    //cyw43_arch_deinit();
    return 0;
}
