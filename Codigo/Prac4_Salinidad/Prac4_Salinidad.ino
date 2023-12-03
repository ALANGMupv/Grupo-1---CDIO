#include <Adafruit_ADS1X15.h>

#define power_PIN 5

int channelSalinidad = 0;

Adafruit_ADS1115 ads1115;

void setup() {
  pinMode(power_PIN, OUTPUT);
  Serial.begin(9600);
  ads1115.begin(0x48);
  ads1115.setGain(GAIN_ONE);
}

void loop() {
  medirSalinidad(channelSalinidad);
  delay(3000);
}

void medirSalinidad(int channelValue) {
  int16_t adc0;

  digitalWrite(power_PIN, HIGH);
  delay(100);

  adc0 = analogRead(channelValue);
  digitalWrite(power_PIN, LOW);
  delay(100);

  Serial.print("Lectura digital = ");
  Serial.println(adc0, DEC);

  // en gramos
  float grSal = calcularSalinidad(adc0);

  Serial.print("Gramos de sal: ");
  Serial.println(grSal);
  Serial.println("........................");
}

float calcularSalinidad(int adc0) {
  
  
  return grSal;
} 
