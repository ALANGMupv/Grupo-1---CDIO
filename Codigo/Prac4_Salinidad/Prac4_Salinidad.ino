#include <Adafruit_ADS1X15.h>

#define power_PIN 5

Adafruit_ADS1115 ads1115;

void setup() {
  pinMode(power_PIN, OUTPUT);
  Serial.begin(9600);
  ads1115.begin(0x48);
  ads1115.setGain(GAIN_ONE);
}

void loop() {
  medirSalinidad();
  delay(3000);
}

void medirSalinidad() {
  int16_t adc0;

  digitalWrite(power_PIN, HIGH);
  delay(100);

  adc0 = ads1115.readADC_SingleEnded(0);
//  adc0 = analogRead(A0);
  digitalWrite(power_PIN, LOW);
  delay(100);

  Serial.print("Lectura digital = ");
  Serial.println(adc0, DEC);

  Serial.print("Gramos de sal: ");
  Serial.println(calcularSalinidad(adc0));
  Serial.println("........................");
}

float calcularSalinidad(int adc0) {
  float grSal = 0.0000007 * pow(adc0, 3) - 0.0006 * pow(adc0, 2) + 0.114 * adc0;
  if (adc0 < 450) { grSal = 0; }
  
  return grSal;
} 
