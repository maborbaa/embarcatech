#include <stdint.h>

volatile uint32_t counter = 0;

int main(void) {
    while (1) {
        counter++;
    }
    return 0;
}
