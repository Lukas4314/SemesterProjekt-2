#pragma once
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"



#define PWM_PIN0 0 // Channel A
#define PWM_PIN1 1 // Channel B

class Gripper
{
public:
    Gripper();
    ~Gripper();
    void open();
    void close();

private:
    uint slice_numPin0;
    uint slice_numPin1;
};