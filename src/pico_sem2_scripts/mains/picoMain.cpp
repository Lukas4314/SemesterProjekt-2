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
#define GETADC 0x09

int main()
{
    stdio_init_all();

    stdio_usb_init();
    while (!stdio_usb_connected())
    {
        sleep_ms(10);
    }

    adc_init();          // Initialize ADC hardware
    adc_select_input(0); // Select ADC input 0 (GPIO 26)

    Gripper gripper;
    __uint8_t status = 0;
    int timeindex = 0;
    int timeIndexForADC = 0;

    while (true)
    {
        __uint8_t ch = getchar_timeout_us(0);
        int adc_value = adc_read(); // Read ADC value
        // Check if the gripper is closed
        if (adc_value >= 430 && status == GRIPPER_CLOSING && timeIndexForADC < 0)
        {
            status = GRIPPER_CLOSED;
            gripper.stop();
        }

        // Check comms from the pc
        if (ch == PICO_ERROR_TIMEOUT)
        {
            // No input, continue
        }
        else if (ch == OPEN_GRIPPER)
        {
            // Status has yet not been set
            if (status == 0)
            {
                // Setting the status to closed so it would opem
                status = GRIPPER_CLOSED;
            }

            if (status == GRIPPER_CLOSED)
            {
                gripper.open();
                putchar(GRIPPER_ACK);
                status = GRIPPER_OPENING;
                timeindex = 500; // Set the time index to 200 ms
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
        else if (ch == CLOSE_GRIPPER)
        {
            // Status has yet not been set
            if (status == 0)
            {
                // Setting the status to open so it would close
                status = GRIPPER_OPEN;
            }

            if (status == GRIPPER_OPEN)
            {
                gripper.close();
                timeIndexForADC = 50;
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
        else if (ch == GRIPPER_STATUS)
        {
            putchar(GRIPPER_ACK);
            putchar(status);
        }
        else if (ch == GETADC)
        {
            putchar(GRIPPER_ACK);
            putchar(adc_value / 5);
        }

        // Check if the gripper is opening
        if (status == GRIPPER_OPENING)
        {
            if (timeindex <= 0)
            {
                gripper.stop();
                status = GRIPPER_OPEN;
            }
        }
        timeindex--;
        timeIndexForADC--;

        sleep_ms(10); // optional: avoid busy loop
    }
    return 0;
}