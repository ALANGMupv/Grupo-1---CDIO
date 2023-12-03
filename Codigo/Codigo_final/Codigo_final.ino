/*************************
 * Codigo final v1.0
 ************************/

#include <Adafruit_ADS1X15.h>

#define channelTemp 0
#define channelHumedad 1

int16_t humedadValue = 0;
int16_t tempValue = 0;

Adafruit_ADS1115 ads1115;

void setup() {
  Serial.begin(9600);
  Serial.println("Inicializando los sensores...");
  ads1115.begin(0x48);
  ads1115.setGain(GAIN_ONE);
  Serial.println("Calibrando los sensores...");
  Serial.println("Sensores calibrados...");
}

void loop() {
  Serial.println("*********************");
  medirHumedad(channelHumedad);
//  tempValue = averageSample(10, channelTemp);
  medirTemperatura(channelTemp);
  delay(2000); 
}

void medirHumedad(int channelValue) {  
  humedadValue = ads1115.readADC_SingleEnded(channelValue);
  int humedadPorcentaje = map(humedadValue, 30000, 16800, 0, 100);
  Serial.print("Humedad: ");
  Serial.println(humedadValue,DEC);
  Serial.print("Porcentaje de humedad: ");
  Serial.print(humedadPorcentaje,DEC);
  Serial.println("%");
}

void medirTemperatura (int channelValue) {
  int16_t adc0 = ads1115.readADC_SingleEnded(channelValue);
  float b = 0.79;
  float m = 0.033;
  float vo = (adc0 * 4.096) / 32767;
//  Serial.print("Valor digital leido: ");
//  Serial.println(vo);
  float temperatura = (vo - b) / m;
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println("º");
}

int16_t averageSample (int ArrayLength, int channelValue) {
  int16_t media = 0;
  for (int i = 0; i < ArrayLength; i++) {
      media += ads1115.readADC_SingleEnded(channelValue);
  }
  
  return media;
}
