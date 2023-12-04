/*************************
 * Codigo final
 ************************/

#include <Adafruit_ADS1X15.h>

#define channelTemp 1
#define channelHumedad 0
#define channelPh 2
#define channelSal 3

int16_t humedadValue = 0;
int16_t mediaHumedad;
int16_t tempValue = 0;
float Offset = 0.95;
int samplingInterval = 20;
int printInterval = 800;
int ArrayLength = 40;
int pHArrayIndex = 0;

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
  int humedadPorcentaje = map(humedadValue, mediaHumedad, 77, 0, 100);
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
  Serial.print(temperatura, DEC);
  Serial.println("º");
}

void medirSalinidad () {
  
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
  float grSal = (adc0 - 575.6) / 6.32;
  
  return grSal;
} 

void medirPh (int channelValue) {
  int pHArray[ArrayLength];
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue, voltage;
  float phSum = 0;

  if (millis() - samplingTime > samplingInterval) {
    pHArray[pHArrayIndex++] = ads1115.readADC_SingleEnded(channelValue);
    phSum += pHArray[pHArrayIndex];
    if (pHArrayIndex == ArrayLength){
        pHArrayIndex = 0;
    } 

    //Serial.print("samplingTime:");
    //Serial.println(samplingTime, DEC);
    //Serial.print("phArrayIndex:");
    //Serial.println(pHArrayIndex, DEC);
    //Serial.print("phArray:");
    //Serial.println(pHArray[pHArrayIndex - 1], DEC);
    //samplingTime = millis();
   

    // phSum += pHArray[pHArrayIndex];
    //if (pHArrayIndex == ArrayLength - 1) {
    //  pHArrayIndex = 0;
    //  Serial.print("phArrayIndex:");
    //  Serial.println(pHArrayIndex, 2);
    //} else {
    //  pHArrayIndex++;
    //}
    // Calcular voltage manualmente usando la resolución del ADC y la referencia de voltaje
    float resolution = 32767.0;      // Resolución del ADC de 15 bits
    float referenceVoltage = 4.096;  // Referencia de voltaje del ADS1115 en voltios
    //Serial.print("Pharrray index");
    //Serial.println(pHArrayIndex); //poner un delay y ver queesta mostrando, no estamos cogiendo el valor de ??
    delay(1000);

    //Serial.println(pHArray[pHArrayIndex]);
    voltage = (phSum) * (referenceVoltage / resolution);
    pHValue = 3.5 * voltage + Offset;

    //Serial.print("Voltage:");
    //Serial.print(voltage, DEC);
    //Serial.print('\n');
    //Serial.print("pHSum:");
    //Serial.println(phSum, DEC);
    //Serial.print("pH:");
    //Serial.println(pHValue, DEC);
    //samplingTime = millis();
    phSum = 0;
}

    if (millis() - printTime > printInterval) {
    Serial.print("Voltage pH:");
    Serial.println(voltage, 2);
//    Serial.print('\n');
//    Serial.print("pHSum:");
//    Serial.println(phSum, 2);
    Serial.print("pH:");
    Serial.println(pHValue, 2);
    printTime = millis();
  }
}


int16_t averageSample (int ArrayLength, int channelValue) {
  int16_t media = 0;
  for (int i = 0; i < ArrayLength; i++) {
      media += ads1115.readADC_SingleEnded(channelValue);
  }
  
  return media;
}
