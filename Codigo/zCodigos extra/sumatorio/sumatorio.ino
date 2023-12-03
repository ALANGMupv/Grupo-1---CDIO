void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
}

void loop() {
  int numero;
  int suma = 0;
  Serial.println("Ingrese un número: ");

  while (true) {
    while (!Serial.available()); 
    numero = Serial.parseInt();

    suma += numero;
    Serial.println("Suma total: " + String(suma));
  }

  
}
