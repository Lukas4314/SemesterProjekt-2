#include "pico/stdlib.h"
#include <stdio.h>

int main() {
    stdio_usb_init();
    while (!stdio_usb_connected()) {
        sleep_ms(10);
    }


    while (true) {
        int ch = getchar_timeout_us(0);
        if (ch != PICO_ERROR_TIMEOUT) {
            uint8_t received = (uint8_t)ch;


            uint8_t response = received + 1;  // do something simple
            putchar(response);               // send 1 byte back
        }

        sleep_ms(10); // optional: avoid busy loop
    }
}
