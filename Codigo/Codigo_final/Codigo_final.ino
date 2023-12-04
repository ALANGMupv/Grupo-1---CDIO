/*************************
 * Codigo final
 ************************/

#include <Adafruit_ADS1X15.h>

#define power_PIN 5
#define channelTemp 1
#define channelHumedad 0
#define channelPh 2
//#define channelSal 2

int16_t humedadValue = 0;
int16_t mediaHumedad;
int16_t tempValue = 0;
float Offset = -0.20;

Adafruit_ADS1115 ads1115;

void setup() {
  Serial.begin(9600);
  ads1115.begin(0x48);
  if (!ads1115.begin()) {
    Serial.println("Fallo al inicializar el ADS");
    while (1)
      ;
  }
  ads1115.setGain(GAIN_ONE);
  Serial.println("Inicializando los sensores...");
  Serial.println("Calibrando los sensores...");
  mediaHumedad = averageSample(30, channelHumedad);
  Serial.println("Sensores calibrados...");
  Serial.println("Iniciando lectura...");
}

void loop() {
  Serial.println("*********************");
  medirHumedad(channelHumedad);
  medirTemperatura(channelTemp);
  medirPh(channelPh);
  medirSalinidad();
//  medirSalinidad(channelSal);
  delay(5000);
}

void medirHumedad(int channelValue) {  
  humedadValue = ads1115.readADC_SingleEnded(channelValue);
  int humedadPorcentaje = map(humedadValue, 30200, 16800, 0, 100);
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
  Serial.print("vo temperatura: ");
  Serial.println(vo);
  float temperatura = (vo - b) / m;
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println("º");
}

void medirSalinidad () {
  
  int16_t adc0;

  digitalWrite(power_PIN, HIGH);
  adc0 = analogRead(A0);
  float grSal = (adc0 - 575.6) / 6.32;

//  adc0 = ads1115.readADC_SingleEnded(0);
//  
//  digitalWrite(power_PIN, LOW);
//  delay(100);

  Serial.print("Lectura sal  = ");
  Serial.println(adc0, DEC);

  Serial.print("Gramos de sal: ");
  Serial.println(grSal);
  Serial.println("........................");

}

/*float calcularSalinidad(int adc0) {
  float grSal = (adc0 - 575.6) / 6.32;
  
  return grSal;
} */

void medirPh (int channelValue) {
  static float pHValue, voltage;
  int lecturaPh = ads1115.readADC_SingleEnded(channelValue);
  float resolution = 32767.0;
  float referenceVoltage = 4.096;  
  voltage = lecturaPh * (referenceVoltage / resolution);
  pHValue = (3.5 * voltage) + Offset;

  Serial.print("Voltaje pH:");
  Serial.println(voltage, 2);
  Serial.print("pH:");
  Serial.println(pHValue, 2);
}


int16_t averageSample (int ArrayLength, int channelValue) {
  int16_t media = 0;
  for (int i = 0; i < ArrayLength; i++) {
      media += ads1115.readADC_SingleEnded(channelValue);
  }
  
  return media;
}
