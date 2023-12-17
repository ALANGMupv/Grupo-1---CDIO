/* Programa prueba GPS UBLOX-6M
 *  Conexiones GPIO 12--> TX
 *             GPIO 13--> RX
 */


#include <SoftwareSerial.h>
#include "TinyGPS++.h"

/*Altitude TinyGPSPlus gps;
  while (ss.available() > 0)
    gps.encode(ss.read());
    if (gps.altitude.isUpdated())
  Serial.println(gps.altitude.meters());

  print 
  Serial.print("LAT=");  Serial.println(gps.location.lat(), 6);
  Serial.print("LONG="); Serial.println(gps.location.lng(), 6);
  Serial.print("ALT=");  Serial.println(gps.altitude.meters());

  $GPRMB,A,4.08,L,EGLL,EGLM,5130.02,N,00046.34,W,004.6,213.9,122.9,A*3D

  With TinyGPS++ it is now possible to extract just the “L” in the third field (it means “steer Left!”). It’s easy with the new TinyGPSCustom watcher object:
  TinyGPSCustom steerDirection(gps, "GPRMB", 3);
  Serial.print(steerDirection.value()); // prints "L" or "R"

  El encode
  SoftwareSerial ss(4, 3);
  void loop()
  {
    while (ss.available() > 0)
      gps.encode(ss.read);

  
  
*/

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
    dato=gps.read();
    Serial.print(dato);
    //delay(5000);
  }
}
