#pragma once
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"



#define PIN14 14 // Channel A
#define PIN15 15 // Channel B

class Gripper
{
public:
    Gripper();
    ~Gripper();
    void open();
    void close();

};