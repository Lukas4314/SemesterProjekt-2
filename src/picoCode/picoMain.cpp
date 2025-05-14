#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "Gripper.h"

int main() {
    stdio_init_all();  // Initialiser USB-seriel
    Gripper gripper;

    printf("Lukker gribber...\n");
    gripper.close();  // Luk gribberen ved opstart

    while (true) {
        uint16_t adc_value = adc_read();  // Læs fra ADC0 (GPIO 26)
        printf("%d\n", adc_value);  // Udskriv ADC-værdien som et heltal
        sleep_ms(500);  // Læs hvert 0.5 sekund
    }

    return 0;
}


    
        

