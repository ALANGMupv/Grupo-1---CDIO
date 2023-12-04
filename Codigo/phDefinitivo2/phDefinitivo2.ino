#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

int channelValue = 0;
float Offset = 0.12;
int samplingInterval = 20;
int printInterval = 800;
int ArrayLength = 40;
int pHArrayIndex = 0;
float phSum = 0;

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


  // Si han pasdo samplingInterval seg desde la última captura
  if (millis() - samplingTime > samplingInterval) {
    // Captura  muestra y acumula
    pHArray[pHArrayIndex++] = ads.readADC_SingleEnded(channelValue);
    phSum = pHArray[pHArrayIndex - 1] + phSum;
    Serial.println("*****Muestreo**********");
    Serial.print("phArrayIndex:");
    Serial.println(pHArrayIndex, DEC);
    Serial.print("phArray:");
    Serial.println(pHArray[pHArrayIndex - 1], DEC);
    Serial.print("phSum:");
    Serial.println(phSum, DEC);


    if (pHArrayIndex == ArrayLength) {

      // Calcula el voltaje y el pH
      float resolution = 32767.0;      // Resolución del ADC de 15 bits
      float referenceVoltage = 4.096;  // Referencia de voltaje del ADS1115 en voltios
      voltage = (phSum / ArrayLength) * (referenceVoltage / resolution);
      pHValue = 3.5 * voltage + Offset;
      Serial.println("****Calculo************");
      Serial.print("Voltaje:");
      Serial.println(voltage, 2);

      Serial.print("samplingTime:");
      Serial.println(samplingTime, DEC);
      Serial.print("phArrayIndex:");
      Serial.println(pHArrayIndex, DEC);
      Serial.print("phArray:");
      Serial.println(pHArray[pHArrayIndex - 1], DEC);
      Serial.print("phSum:");
      Serial.println(phSum, DEC);
      // Inicializa
      pHArrayIndex = 0;
      phSum = 0;
    }
  }



  if (millis() - printTime > printInterval) {
    Serial.println("/////Imprimo /////////////");
    Serial.print("Voltaje:");
    Serial.println(voltage, 2);
    Serial.print("pH:");
    Serial.println(pHValue, 2);
    printTime = millis();
  }
}
