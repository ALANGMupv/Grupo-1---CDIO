#ifndef HumiditySensor_h
#define HumiditySensor_h

#include <Adafruit_ADS1X15.h>

class HumiditySensor {
public:
    HumiditySensor(Adafruit_ADS1115& ads, int channelValue);
    int measureHumidity();

private:
    Adafruit_ADS1115& ads;
    int channelValue;
};

#endif
