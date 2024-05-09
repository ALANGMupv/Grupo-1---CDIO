// PhSensor.cpp

#include "PhSensor.h"

PhSensor::PhSensor(Adafruit_ADS1115& ads, int channelValue, float phOffset) : ads(ads), channelValue(channelValue), phOffset(phOffset) {}

float PhSensor::measurePh() {
    static float pHValue, voltage;
    int phReading = ads.readADC_SingleEnded(channelValue);
    float resolution = 32767.0;
    float referenceVoltage = 4.096;  
    voltage = phReading * (referenceVoltage / resolution);
    pHValue = (3.5 * voltage) + phOffset;

    Serial.print("Voltaje pH:");
    Serial.println(voltage, 2);
    Serial.print("pH:");
    Serial.println(pHValue, 2);

    return pHValue;
}
