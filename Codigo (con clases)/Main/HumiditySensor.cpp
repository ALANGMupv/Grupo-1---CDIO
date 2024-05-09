// HumiditySensor.cpp

#include "HumiditySensor.h"

HumiditySensor::HumiditySensor(Adafruit_ADS1115& ads, int channelValue) : ads(ads), channelValue(channelValue) {}

int HumiditySensor::measureHumidity() {
    int humedadValue = ads.readADC_SingleEnded(channelValue);
    int humedadPorcentaje = map(humedadValue, 30200, 16800, 0, 100);

    Serial.print("Humedad: ");
    Serial.println(humedadValue, DEC);
    Serial.print("Porcentaje de humedad: ");
    Serial.print(humedadPorcentaje, DEC);
    Serial.println("%");

    return humedadPorcentaje;
}
