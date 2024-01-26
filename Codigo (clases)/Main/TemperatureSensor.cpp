// TemperatureSensor.cpp

#include "TemperatureSensor.h"

TemperatureSensor::TemperatureSensor(Adafruit_ADS1115& ads, int channelValue) : ads(ads), channelValue(channelValue) {}

float TemperatureSensor::measureTemperature() {
    int16_t adcValue = ads.readADC_SingleEnded(channelValue);
    float b = 0.79;
    float m = 0.033;
    float vo = (adcValue * 4.096) / 32767;
    Serial.print("vo temperatura: ");
    Serial.println(vo);
  
    float temperature = (vo - b) / m;
    Serial.print("Temperatura: ");
    Serial.print(temperature);
    Serial.println("º");

    return temperature;
}
