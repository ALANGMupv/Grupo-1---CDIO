  /*********************************
 * Practica 2: ADC y Comunicacion serie
 *********************************/
#define LED 5

int sensorPin = A0;
int sensorValue = 0;


void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
}

void loop() {
  // leemos el valor del ADC
  sensorValue = analogRead(sensorPin);

  // Imprimimos el valor leido
  Serial.println();
  Serial.print("Voltage: ");
  // sensorValue = map(sensorValue, 0, 1023, 0, 1000);
  // funcion matematica que usariamos si no se pudiese usar map():
  sensorValue = (sensorValue - 0) * (1000 - 0) / (1023 - 0) + 0;
  if(sensorValue > 500) {
      digitalWrite(LED, HIGH);
    } else {
      digitalWrite(LED, LOW);
    }
  Serial.print(sensorValue , DEC);

  delay(1000);
}
