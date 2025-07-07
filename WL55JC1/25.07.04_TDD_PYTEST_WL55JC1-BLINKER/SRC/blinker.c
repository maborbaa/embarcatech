// SRC/blinker.c
#include "blinker.h"
#include "hal_gpio.h"

static Blinker_State_t internal_state;

// Ponteiro para a função que controla o LED
static void (*led_set_func_ptr)(int) = 0; //inject mock - testar o led ON

void blinker_init( void){
    // Inicializa o estado interno como desligado
    internal_state = BLINKER_STATE_OFF;

    //Garantimos que a função de hardware seja chamada - inject mock - testar o led ON
    if(led_set_func_ptr){
        led_set_func_ptr(internal_state);
    }
}

void blinker_tick(void){
    if(internal_state == BLINKER_STATE_OFF){
        internal_state = BLINKER_STATE_ON;
    } else {
        internal_state = BLINKER_STATE_OFF;
    }

    if(led_set_func_ptr){
        led_set_func_ptr(internal_state);
    }
}

Blinker_State_t blinker_get_state(void){
    // Retorna o estado atual do Blinker
    return internal_state;
}

//Permite inject a função mock
void blinker_register_led_func(void (*func)(int)){
    led_set_func_ptr = func;
}
