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
    __uint8_t status = 0;

    while (true) {
        __uint8_t ch = getchar_timeout_us(0);

        if (ch == PICO_ERROR_TIMEOUT) {
            // No input, continue
        }
        else if (ch == OPEN_GRIPPER) {
            
            if (status == GRIPPER_CLOSED)
            {
                gripper.open();
                putchar(GRIPPER_ACK);
                status = GRIPPER_OPENING;
            }
            else if (status == GRIPPER_OPEN)
            {
                putchar(GRIPPER_NACK);
            }
            else if (status == GRIPPER_OPENING)
            {
                putchar(GRIPPER_NACK);
            }
            else if (status == GRIPPER_ERROR)
            {
                putchar(GRIPPER_NACK);
            }
        }
        else if (ch == CLOSE_GRIPPER) {
            
            if (status == GRIPPER_OPEN)
            {
                gripper.close();
                putchar(GRIPPER_ACK);
                status = GRIPPER_CLOSING;
            }
            else if (status == GRIPPER_CLOSED)
            {
                putchar(GRIPPER_NACK);
            }
            else if (status == GRIPPER_OPENING)
            {
                putchar(GRIPPER_NACK);
            }
            else if (status == GRIPPER_ERROR)
            {
                putchar(GRIPPER_NACK);
            }
            
            
        }
        else if (ch == GRIPPER_STATUS) {
            putchar(status);
        }

        if (adc.read() < 100 && status == GRIPPER_CLOSING) {
            status = GRIPPER_CLOSED;
            gripper.stop();
        }
        
        
        
        


        // if (ch != PICO_ERROR_TIMEOUT) {
        //     if (ch == 1){
        //         gripper.open();
        //     }
        //     else if (ch == 2){
        //         gripper.close();
        //     }
        // }
        sleep_ms(10); // optional: avoid busy loop
    }
    return 0;
}