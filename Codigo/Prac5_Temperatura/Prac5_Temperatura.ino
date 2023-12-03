#include <Adafruit_ADS1X15.h>
#define channelValue 0

Adafruit_ADS1115 ads1115;

void setup() {
  Serial.begin(9600);
  Serial.println("Inicializando el medidor de Temperatura");
  ads1115.begin(0x48);
  ads1115.setGain(GAIN_ONE);
}

void loop() {
  medirTemperatura(0);
  delay(2000);
}

void medirTemperatura (int sensorValue) {
  //int16_t adc0 = ads1115.readADC_SingleEnded(sensorValue);
  int16_t adc0 = averageSample(10, sensorValue);
  float b = 0.79;
  float m = 0.033;
  float vo = (adc0 * 4.096) / 32767;
  Serial.print("Valor digital leido: ");
  Serial.println(vo);
  float temperatura = (vo - b) /m;
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println("º");
}

int16_t averageSample (int ArrayLength, int sensorValue) {
  int16_t media = 0;
  for (int i = 0; i < ArrayLength; i++) {
      media =+ ads1115.readADC_SingleEnded(sensorValue);
  }
  
  return media;
}
