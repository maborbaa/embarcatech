/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

// Definição dos pinos
#define BUTTON_PIN 5      // Botão no GPIO5
#define LED_RED_PIN 13    // Vermelho (GPIO13)
#define LED_GREEN_PIN 12  // Verde (GPIO12)
#define LED_BLUE_PIN 11   // Azul (GPIO11)
#define JOYSTICK_X_PIN 26 // Eixo X (ADC0 - GPIO26)
#define JOYSTICK_Y_PIN 27 // Eixo Y (ADC1 - GPIO27)

// Modos de operação
typedef enum {
    MODE_RED_GREEN,  // X controla Vermelho, Y controla Verde
    MODE_BLUE_RED,   // X controla Azul, Y controla Vermelho
    MODE_GREEN_BLUE, // X controla Verde, Y controla Azul
    NUM_MODES
} ControlMode;

ControlMode current_mode = MODE_RED_GREEN;

// Inicialização do PWM para os LEDs RGB
void setup_rgb_pwm() {
    gpio_set_function(LED_RED_PIN, GPIO_FUNC_PWM);
    gpio_set_function(LED_GREEN_PIN, GPIO_FUNC_PWM);
    gpio_set_function(LED_BLUE_PIN, GPIO_FUNC_PWM);

    // Configuração do PWM para cada slice
    pwm_set_wrap(pwm_gpio_to_slice_num(LED_RED_PIN), 65535);
    pwm_set_wrap(pwm_gpio_to_slice_num(LED_GREEN_PIN), 65535);
    pwm_set_wrap(pwm_gpio_to_slice_num(LED_BLUE_PIN), 65535);

    pwm_set_enabled(pwm_gpio_to_slice_num(LED_RED_PIN), true);
    pwm_set_enabled(pwm_gpio_to_slice_num(LED_GREEN_PIN), true);
    pwm_set_enabled(pwm_gpio_to_slice_num(LED_BLUE_PIN), true);
}

// Atualiza as cores com base no modo e posição do joystick
void update_led_color(uint16_t x_value, uint16_t y_value) {
    switch (current_mode) {
        case MODE_RED_GREEN:
            pwm_set_gpio_level(LED_RED_PIN, x_value);
            pwm_set_gpio_level(LED_GREEN_PIN, y_value);
            pwm_set_gpio_level(LED_BLUE_PIN, 0);
            break;
            
        case MODE_BLUE_RED:
            pwm_set_gpio_level(LED_BLUE_PIN, x_value);
            pwm_set_gpio_level(LED_RED_PIN, y_value);
            pwm_set_gpio_level(LED_GREEN_PIN, 0);
            break;
            
        case MODE_GREEN_BLUE:
            pwm_set_gpio_level(LED_GREEN_PIN, x_value);
            pwm_set_gpio_level(LED_BLUE_PIN, y_value);
            pwm_set_gpio_level(LED_RED_PIN, 0);
            break;
    }
}

int main() {
    stdio_init_all();
    adc_init();

    // Configuração do ADC para o joystick
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);

    // Configuração do botão
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    // Configuração do LED RGB
    setup_rgb_pwm();

    uint64_t last_button_press = 0;

    while (1) {
        // Leitura do joystick
        adc_select_input(0); // Canal ADC0 (X)
        uint16_t x_value = adc_read();
        adc_select_input(1); // Canal ADC1 (Y)
        uint16_t y_value = adc_read();

        // ==== ADICIONE AQUI ==== //
        // Converte ADC (12 bits) para PWM (16 bits):
        x_value = (x_value << 4); // 4095 → 65520 (próximo de 65535)
        y_value = (y_value << 4);
        // ======================= //

        // Atualiza as cores
        update_led_color(x_value, y_value);

        // Verifica pressionamento do botão (com debounce)
        if (!gpio_get(BUTTON_PIN)) {
            if (to_ms_since_boot(get_absolute_time()) - last_button_press > 200) {
                current_mode = (current_mode + 1) % NUM_MODES;
                last_button_press = to_ms_since_boot(get_absolute_time());
            }
        }

        sleep_ms(50);
    }
}