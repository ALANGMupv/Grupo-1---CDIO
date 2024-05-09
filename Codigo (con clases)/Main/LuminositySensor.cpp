// LuminositySensor.cpp

#include "LuminositySensor.h"

LuminositySensor::LuminositySensor(Adafruit_ADS1115& ads, int channelValue) : ads(ads), channelValue(channelValue) {}

int16_t LuminositySensor::measureLuminosity() {
    int16_t adcValue = ads.readADC_SingleEnded(channelValue);

    Serial.println(adcValue);
    if (adcValue <= 400 ) {
        Serial.println("Luminosidad: oscuro");
    } else if (adcValue <= 1100 ) {
        Serial.println("Luminosidad: sombra");
    } else if (adcValue <= 4000 ) {
        Serial.println("Luminosidad: luz ambiente");
    } else if (adcValue <=30200 ) {
        Serial.println("Luminosidad: luz de móvil");
    } 

    return adcValue;
}
