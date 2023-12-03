#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

int channelValue = 0;
float Offset = 0.95;
int samplingInterval = 20;
int printInterval = 800;
int ArrayLength = 40;
int pHArrayIndex = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Inicializando medidor de pH");

  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1)
      ;
  }

  ads.setGain(GAIN_ONE);
}

void loop() {
  int pHArray[ArrayLength];
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue, voltage;
  float phSum = 0;

  if (millis() - samplingTime > samplingInterval) {
    pHArray[pHArrayIndex++] = ads.readADC_SingleEnded(channelValue);
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
    Serial.print("Voltage:");
    Serial.print(voltage, 2);
    Serial.print('\n');
    Serial.print("pHSum:");
    Serial.println(phSum, 2);
    Serial.print("pH:");
    Serial.println(pHValue, 2);
    printTime = millis();
  }  }
  
