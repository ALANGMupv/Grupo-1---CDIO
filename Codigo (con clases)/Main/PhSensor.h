// PhSensor.h

#ifndef PhSensor_h
#define PhSensor_h

#include <Adafruit_ADS1X15.h>

class PhSensor {
public:
    PhSensor(Adafruit_ADS1115& ads, int channelValue, float phOffset);
    float measurePh();

private:
    Adafruit_ADS1115& ads;
    int channelValue;
    float phOffset;
};

#endif
