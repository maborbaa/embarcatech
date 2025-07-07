// SRC/blinker.h
#ifndef BLINKER_H
#define BLINKER_H

typedef enum {
    BLINKER_STATE_OFF = 0, // Desligado
    BLINKER_STATE_ON = 1   // Ligado
} Blinker_State_t;

void blinker_init(void);

void blinker_tick(void);
Blinker_State_t blinker_get_state(void);

#endif // BLINKER_H

void blinker_register_led_func(void (*func)(int)); //inject mock
