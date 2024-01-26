// LuminositySensor.h

#ifndef LuminositySensor_h
#define LuminositySensor_h

#include <Adafruit_ADS1X15.h>

class LuminositySensor {
public:
    LuminositySensor(Adafruit_ADS1115& ads, int channelValue);
    int16_t measureLuminosity();

private:
    Adafruit_ADS1115& ads;
    int channelValue;
};

#endif
