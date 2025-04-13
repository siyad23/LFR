#include <Arduino.h>

// handle the rotary encoder and switch
#include "switch.h"

// handle the display. Don't change this include order.
#include "logo.h"
#include <Org_01.h>
#include "display.h"

void basic_init()
{                       // Initialize the rotary encoder and switch
    Serial.begin(9600); // Initialize serial communication at 9600 baud rate
}