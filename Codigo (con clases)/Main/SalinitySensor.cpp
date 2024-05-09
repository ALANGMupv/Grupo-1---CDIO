// SalinitySensor.cpp

#include "SalinitySensor.h"

SalinitySensor::SalinitySensor(int powerPin) : powerPin(powerPin) {}

float SalinitySensor::measureSalinity() {
    int16_t adc0;

    digitalWrite(powerPin, HIGH);
    delay(100);

  
    adc0 = (analogRead (A0))-400;
    digitalWrite(powerPin, LOW);
    delay(100);

    float grSal = 0.0000007 * pow(adc0, 3) - 0.0006 * pow(adc0, 2) + 0.114 * adc0;
    if (adc0 < 580) { 
        grSal = 0; 
    }

    return grSal;
}
