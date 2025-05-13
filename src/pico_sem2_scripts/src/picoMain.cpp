#include "Gripper.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"


Gripper::Gripper()
{
   // Initialiser ADC
   adc_init();
   adc_gpio_init(26); // GPIO 26 = ADC0
   adc_select_input(0);

   // Set both pins to PWM function
   gpio_set_function(PWM_PIN0, GPIO_FUNC_PWM);
   gpio_set_function(PWM_PIN1, GPIO_FUNC_PWM);




   // Get slice number (same for both since pin 0 and 1 are on the same slice)
   uint slice_numPin0 = pwm_gpio_to_slice_num(PWM_PIN0);
   uint slice_numPin1 = pwm_gpio_to_slice_num(PWM_PIN1);

   // Set PWM frequency to 50Hz (20ms period)
   pwm_set_wrap(slice_numPin0, 10);       // 1 MHz counter, so 20 ms period = 20000 counts
   pwm_set_wrap(slice_numPin1, 10);       // 1 MHz counter, so 20 ms period = 20000 counts

   pwm_set_clkdiv(slice_numPin0, 125.0f); // 125MHz / 125 = 1MHz
   pwm_set_clkdiv(slice_numPin1, 125.0f); // 125MHz / 125 = 1MHz


   pwm_set_enabled(slice_numPin0, true);
   pwm_set_enabled(slice_numPin1, true);

}
Gripper::~Gripper()
{
    // Destructor implementation
}


void Gripper::open(){
    pwm_set_chan_level(slice_numPin0, PWM_CHAN_A, 5); // Pin 0 - 1ms pulse (open)
    sleep_ms(1000);
    pwm_set_chan_level(slice_numPin0, PWM_CHAN_A, 0); // Pin 0 - 1ms pulse (open)
    putchar(1);

}
void Gripper:close(){
    pwm_set_chan_level(slice_numPin1, PWM_CHAN_A, 5); // Pin 0 - 2ms pulse (close)

    while (adc.read() < 100) {
        
        sleep_ms(10);
    }

    pwm_set_chan_level(slice_numPin1, PWM_CHAN_A, 0); // Pin 0 - 2ms pulse (close)
    putchar(1);
}