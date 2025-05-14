#include "Gripper.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"

#define PIN0 0
#define PIN1 1

Gripper::Gripper()
{
    // Initialiser ADC
    adc_init();
    adc_gpio_init(26); // GPIO 26 = ADC0
    adc_select_input(0);

    gpio_init(PIN0);
    gpio_init(PIN1);

    gpio_set_dir(PIN0, GPIO_OUT);
    gpio_set_dir(PIN1, GPIO_OUT);

    gpio_put(PIN0, 0);
    gpio_put(PIN1, 0);
}
Gripper::~Gripper()
{
    // Destructor implementation
}

void Gripper::open()
{
    gpio_put(PIN0, 1);
    sleep_ms(1000);
    gpio_put(PIN0, 0);
    putchar(1);
}
void Gripper::close()
{
    gpio_put(PIN1, 1);

    while (adc.read() < 100)
    {

        sleep_ms(10);
    }

    gpio_put(PIN1, 0);
    putchar(1);
}