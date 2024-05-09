#ifndef TemperatureSensor_h
#define TemperatureSensor_h

#include <Adafruit_ADS1X15.h>

class TemperatureSensor {
public:
    TemperatureSensor(Adafruit_ADS1115& ads, int channelValue);
    float measureTemperature();

private:
    Adafruit_ADS1115& ads;
    int channelValue;
};

#endif
