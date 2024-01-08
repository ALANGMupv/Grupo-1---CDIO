#include <Adafruit_ADS1X15.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include "TinyGPS++.h"

#include "WifiConnection.h"
#include "HTTPCommunication.h"
#include "HumiditySensor.h"
#include "TemperatureSensor.h"
#include "SalinitySensor.h"
#include "PhSensor.h"
#include "LuminositySensor.h"
#include "GPSModule.h"

Adafruit_ADS1115 ads1115;
WiFiConnection wifiConnection("wifi", "pandi123");
HumiditySensor humiditySensor(ads1115, 0);
TemperatureSensor temperatureSensor(ads1115, 1);
SalinitySensor salinitySensor(5);  // Ajusta el valor del pin de alimentación según tu configuración
PhSensor phSensor(ads1115, 2, -0.20);  // Ajusta el valor del offset según tu sensor de pH
LuminositySensor luminositySensor(ads1115, 3);
HTTPCommunication httpCommunication("api.thingspeak.com", 80);
SoftwareSerial gpsSerial(12, 13);
GPSModule gpsModule(gpsSerial);

#ifdef REST_SERVER_THINGSPEAK 
  const char Rest_Host[] = "api.thingspeak.com";
  String MyWriteAPIKey="53NUJ2ME5T7NMVFL"; // clave de canal ThingSpeak
#else 
  const char Rest_Host[] = "dweet.io";
  String MyWriteAPIKey="cdiocurso2023g01"; // clave de canal Dweet
#endif

WiFiClient client;

void setup() {
  Serial.begin(9600);
  
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);

  wifiConnection.connect();
}

void loop() {
    int humidity = humiditySensor.measureHumidity();
    float temperature = temperatureSensor.measureTemperature();
    float salinity = salinitySensor.measureSalinity();
    float pH = phSensor.measurePh();
    int16_t luminosity = luminositySensor.measureLuminosity();

    // Envía datos a través de HTTP
    String data[5];  // Número de sensores
    data[0] = String(humidity);
    data[1] = String(temperature);
    data[2] = String(salinity);
    data[3] = String(pH);
    data[4] = String(luminosity);

    httpCommunication.HTTPGet(data, 5);

    // Lee datos del módulo GPS
    gpsModule.readGPSData();

    delay(5000);
} 

void connectWiFi() {
  #ifdef PRINT_DEBUG_MESSAGES
    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());
  #endif
  
  WiFi.begin(WiFiSSID, WiFiPSK);

  while (WiFi.status() != WL_CONNECTED)  {
    #ifdef PRINT_DEBUG_MESSAGES
       Serial.println(".");
    #endif
    delay(500);
  }
  #ifdef PRINT_DEBUG_MESSAGES
     Serial.println( "WiFi Connected" );
     Serial.println(WiFi.localIP()); // Print the IP address
  #endif
}

/////////////////////////////////////////////////////
/////////////// HTTP POST  ThingSpeak////////////////
//////////////////////////////////////////////////////

void HTTPPost(String fieldData[], int numFields) {

// Esta funcion construye el string de datos a enviar a ThingSpeak mediante el metodo HTTP POST
// La funcion envia "numFields" datos, del array fieldData.
// Asegurate de ajustar numFields al número adecuado de datos que necesitas enviar y activa los campos en tu canal web
  
    if (client.connect( Server_Host , Server_HttpPort )){
       
        // Construimos el string de datos. Si tienes multiples campos asegurate de no pasarte de 1440 caracteres
   
        String PostData= "api_key=" + MyWriteAPIKey ;
        for ( int field = 1; field < (numFields + 1); field++ ){
            PostData += "&field" + String( field ) + "=" + fieldData[ field ];
        }     
        
        // POST data via HTTP
        #ifdef PRINT_DEBUG_MESSAGES
            Serial.println( "Connecting to ThingSpeak for update..." );
        #endif
        client.println( "POST http://" + String(Rest_Host) + "/update HTTP/1.1" );
        client.println( "Host: " + String(Rest_Host) );
        client.println( "Connection: close" );
        client.println( "Content-Type: application/x-www-form-urlencoded" );
        client.println( "Content-Length: " + String( PostData.length() ) );
        client.println();
        client.println( PostData );
        #ifdef PRINT_DEBUG_MESSAGES
            Serial.println( PostData );
            Serial.println();
            //Para ver la respuesta del servidor
            #ifdef PRINT_HTTP_RESPONSE
              delay(500);
              Serial.println();
              while(client.available()){String line = client.readStringUntil('\r');Serial.print(line); }
              Serial.println();
              Serial.println();
            #endif
        #endif
    }
}

////////////////////////////////////////////////////
/////////////// HTTP GET  ////////////////
//////////////////////////////////////////////////////

void HTTPGet(String fieldData[], int numFields){
  
// Esta funcion construye el string de datos a enviar a ThingSpeak o Dweet mediante el metodo HTTP GET
// La funcion envia "numFields" datos, del array fieldData.
// Asegurate de ajustar "numFields" al número adecuado de datos que necesitas enviar y activa los campos en tu canal web
  
    if (client.connect( Server_Host , Server_HttpPort )){
           #ifdef REST_SERVER_THINGSPEAK 
              String PostData= "GET https://api.thingspeak.com/update?api_key=";
              PostData= PostData + MyWriteAPIKey ;
           #else 
              String PostData= "GET http://dweet.io/dweet/for/";
              PostData= PostData + MyWriteAPIKey +"?" ;
           #endif
           
           for ( int field = 1; field < (numFields + 1); field++ ){
              PostData += "&field" + String( field ) + "=" + fieldData[ field ];
           }
          
           
           #ifdef PRINT_DEBUG_MESSAGES
              Serial.println( "Connecting to Server for update..." );
           #endif
           client.print(PostData);         
           client.println(" HTTP/1.1");
           client.println("Host: " + String(Rest_Host)); 
           client.println("Connection: close");
           client.println();
           #ifdef PRINT_DEBUG_MESSAGES
              Serial.println( PostData );
              Serial.println();
              //Para ver la respuesta del servidor
              #ifdef PRINT_HTTP_RESPONSE
                delay(500);
                Serial.println();
                while(client.available()){String line = client.readStringUntil('\r');Serial.print(line); }
                Serial.println();
                Serial.println();
              #endif
           #endif  
    }
}
