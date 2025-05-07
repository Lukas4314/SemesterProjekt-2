#include <stdio.h>
#include "pico/stdlib.h"

int main() {
    // Initialize stdio to use USB UART
    stdio_init_all();

    // Wait for USB serial connection
    sleep_ms(2000); // Wait a bit for connection to establish (optional)

    while (true) {
        printf("Hello, World!\n");
        sleep_ms(1000); // Print every second
    }

    return 0;
}
