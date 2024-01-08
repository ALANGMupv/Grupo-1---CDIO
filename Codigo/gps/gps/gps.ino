/* Programa prueba GPS UBLOX-6M
 *  Conexiones GPIO 12--> TX
 *             GPIO 13--> RX
 */


#include <SoftwareSerial.h>
#include "TinyGPS++.h"

SoftwareSerial gps(12,13);

char dato=' ';

void setup()
{
 Serial.begin(115200);            
 gps.begin(9600); 
 Serial.println("Inicializando el GPS...");
 delay(27000);
 Serial.println("Esperando datos");
}


void loop()
{
  if(gps.available())
  {
    String nmeaSentence = gps.readStringUntil('\n');
    
    if (nmeaSentence.startsWith("$GPRMC")) {
      char validity;
      float latitude, longitude, speed, course;
      int hour, minute, second, day, month, year;

      int parsed = sscanf(nmeaSentence.c_str(), "$GPRMC,%2d%2d%2d.%3d,%c,%f,%f,%c,%f,%f,%6d,,,", &hour, &minute, &second, &day, &validity, &latitude, &longitude, &speed, &course, &year);

      if (parsed == 10) {
        Serial.print("Hora: ");
        Serial.print(hour);
        Serial.print(":");
        Serial.print(minute);
        Serial.print(":");
        Serial.print(second);
        Serial.println();

        Serial.print("Fecha: ");
        Serial.print(day);
        Serial.print("/");
        Serial.print(month);
        Serial.print("/");
        Serial.println(year);

        Serial.print("Validez: ");
        Serial.println(validity);

        Serial.print("Latitud: ");
        Serial.print(latitude, 6);
        Serial.println();

        Serial.print("Longitud: ");
        Serial.print(longitude, 6);
        Serial.println();

        Serial.print("Velocidad: ");
        Serial.print(speed);
        Serial.println(" nudos");

        Serial.print("Orientación: ");
        Serial.print(course);
        Serial.println(" grados");

        Serial.println("-----------------------------");
      }
    }
  }
}
