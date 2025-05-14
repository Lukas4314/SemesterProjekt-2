#include "Gripper.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"


Gripper::Gripper()
{
   // Initialiser ADC
   adc_init();
   adc_gpio_init(26); // GPIO 26 = ADC0
   adc_select_input(0);

   // Pin configuration
    gpio_init(PIN14);
    gpio_set_dir(PIN14, GPIO_OUT);

    gpio_init(PIN15);
    gpio_set_dir(PIN15, GPIO_OUT);

}
Gripper::~Gripper()
{
    // Destructor implementation
}


void Gripper::open(){
    gpio_put(PIN14, 1);
//   sleep_ms(2000);
//    gpio_put(PIN0, 0);
}

void Gripper::close(){
    gpio_put(PIN14, 1);
}

void Gripper::stop()
{
    gpio_put(PIN14, 0);
    gpio_put(PIN15, 0);
}