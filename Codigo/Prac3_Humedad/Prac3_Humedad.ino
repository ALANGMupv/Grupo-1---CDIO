#include <Adafruit_ADS1X15.h>

·define power_PIN 5

int channelValue = 0;
int sensorValue = 0;
int humedadValor = 0;
int humedadPorcentaje = 0;

Adafruit_ADS1115 ads1115;

void setup() {
  Serial.begin(9600);
  ads1115.begin(0x48);
  ads1115.setGain(GAIN_ONE);
}

void loop() {
  medirHumedad(channelValue);
  delay(1000);
}

void medirHumedad(int channelValue) {
  sensorValue = ads1115.readADC_SingleEnded(channelValue);
  humedadValor = sensorValue;
  humedadPorcentaje = map(sensorValue, 29900, 17000, 0, 100);

  Serial.print("Humedad: ");
  Serial.println(humedadValor,DEC);
  Serial.print("Porcentaje: ");
  Serial.print(humedadPorcentaje,DEC);
  Serial.println("%");
  Serial.println("\**************/");
}
