// SalinitySensor.h

#ifndef SalinitySensor_h
#define SalinitySensor_h

#include <Adafruit_ADS1X15.h>

class SalinitySensor {
public:
    SalinitySensor(int powerPin);
    float measureSalinity();

private:
    int powerPin;
};

#endif
