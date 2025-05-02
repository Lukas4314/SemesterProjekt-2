#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "Gripper.h"

int main()
{
    stdio_init_all();


    stdio_usb_init();
    while (!stdio_usb_connected()) {
        sleep_ms(10);
    }

    Gripper gripper;


    while (true) {
        int ch = getchar_timeout_us(0);
        if (ch != PICO_ERROR_TIMEOUT) {
            if (ch == 1){
                gripper.open();
            }
            else if (ch == 2){
                gripper.close();
            }
        }
        sleep_ms(10); // optional: avoid busy loop
    }
    return 0;
}
