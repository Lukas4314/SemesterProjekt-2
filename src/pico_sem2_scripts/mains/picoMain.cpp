#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "Gripper.h"

#define BAUD_RATE B115200
#define CLOSE_GRIPPER 0x01
#define OPEN_GRIPPER 0x02
#define GRIPPER_STATUS 0x03
#define GRIPPER_OPENING 0x13
#define GRIPPER_CLOSING 0x14
#define GRIPPER_ERROR 0x04
#define GRIPPER_ACK 0x06
#define GRIPPER_NACK 0x15
#define GRIPPER_OPEN 0x07
#define GRIPPER_CLOSED 0x08

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