#define ESP8266_LED 5


void setup() {
  // put your setup code here, to run once:
  pinMode(ESP8266_LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(ESP8266_LED, HIGH);
  delay(500);
  digitalWrite(ESP8266_LED, LOW);
  delay(500);
}
