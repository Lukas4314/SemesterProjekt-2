#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"

#define PWM_PIN0 0 // Channel A

int main()
{
    stdio_init_all();

    // Initialiser ADC
    adc_init();
    adc_gpio_init(26); // GPIO 26 = ADC0
    adc_select_input(0);

    // Set both pins to PWM function
    gpio_set_function(PWM_PIN0, GPIO_FUNC_PWM);

    // Get slice number (same for both since pin 0 and 1 are on the same slice)
    uint slice_num = pwm_gpio_to_slice_num(PWM_PIN0);

    // Set PWM frequency to 50Hz (20ms period)
    pwm_set_wrap(slice_num, 10);       // 1 MHz counter, so 20 ms period = 20000 counts
    pwm_set_clkdiv(slice_num, 125.0f); // 125MHz / 125 = 1MHz

    pwm_set_enabled(slice_num, true);

    // Control both channels (e.g., two servos or two motor directions)

    // Example: Pin 0 - open gripper, Pin 1 - close gripper
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 5); // Pin 0 - 1ms pulse (open)
    sleep_ms(1000);

    
    while (true)
    {
        uint16_t result = adc_read(); // Læs ADC (0-4095)
        printf("ADC Value: %d\n", result);

        sleep_ms(10);
    }
        

    return 0;
}
