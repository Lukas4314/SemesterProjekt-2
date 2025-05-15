#include "Gripper.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"


Gripper::Gripper()
{
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
    stop();
    gpio_put(PIN14, 1);
//   sleep_ms(2000);
//    gpio_put(PIN0, 0);
}

void Gripper::close(){
    stop();
    gpio_put(PIN15, 1);
}

void Gripper::stop()
{
    gpio_put(PIN14, 0);
    gpio_put(PIN15, 0);
}