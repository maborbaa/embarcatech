// Dentro de Core/Src/hal_gpio_impl.c
#include "main.h" // Inclui o header que tem o User Label do LED
#include "hal_gpio.h" // Inclui nossa definição de interface

void hal_gpio_led_set(int state) {
    // Converte nosso estado (0 ou 1) para o tipo da HAL da ST
    GPIO_PinState pin_state = (state == 1) ? GPIO_PIN_SET : GPIO_PIN_RESET;

    // Chama a função REAL da HAL da ST para ligar/desligar o pino físico
    HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, pin_state);
}
