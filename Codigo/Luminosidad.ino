#include <Adafruit_ADS1X15.h>

// Declarar el objeto del ADS1115
Adafruit_ADS1115 ads1115;

void setup() {
  Serial.begin(9600); 
  // Configurar correctamente el ADS1115
  ads1115.begin(0x48);
  ads1115.setGain(GAIN_ONE);

}

void loop() {
  // Leer del canal del ADS1115
  int16_t adc0 = ads1115.readADC_SingleEnded(0);
  float voltage = (adc0*4.096)/32767;
  Serial.print("Nivel de luminosidad: ");
  Serial.println(adc0,DEC);
  Serial.print("Voltage luminosidad: ");
  Serial.println(voltage,DEC);

  if (adc0 <= 200 ) {
    Serial.println("Luminosidad: oscuro");
  } else if (adc0 <= 1100 ) {
    Serial.println("Luminosidad: sombra");
  } else if (adc0 <= 3500 ) {
    Serial.println("Luminosidad: luz ambiente");
  } else if (adc0 <=30200 ) {
    Serial.println("Luminosidad: luz de movil");
  } 

  delay(1000);  // Puedes ajustar el tiempo de muestreo según sea necesario
}
