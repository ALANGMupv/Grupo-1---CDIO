#include <Adafruit_ADS1X15.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include "TinyGPS++.h"

// mensajes de depuracion en monitor serie 
#define PRINT_DEBUG_MESSAGES
// respuesta del HTTP server
//#define PRINT_HTTP_RESPONSE

// conexion Fuera/Dentro de UPV
//#define WiFi_CONNECTION_UPV

// servidor REST (ThingSpeak/Dweet)
#define REST_SERVER_THINGSPEAK //https://thingspeak.com/channels/2358189
//#define REST_SERVER_DWEET //https://dweet.io/follow/cdiocurso2023g01

///////////////////////////////////////////////////////
/////////////// WiFi Definitions /////////////////////
//////////////////////////////////////////////////////

#ifdef WiFi_CONNECTION_UPV //Conexion UPV
  const char WiFiSSID[] = "GTI1";
  const char WiFiPSK[] = "1PV.arduino.Toledo";
#else //Conexion fuera de la UPV
  const char WiFiSSID[] = "Imanol";
  const char WiFiPSK[] = "imanolf121";
#endif



///////////////////////////////////////////////////////
/////////////// SERVER Definitions /////////////////////
//////////////////////////////////////////////////////

#if defined(WiFi_CONNECTION_UPV) //Conexion UPV
  const char Server_Host[] = "proxy.upv.es";
  const int Server_HttpPort = 8080;
#elif defined(REST_SERVER_THINGSPEAK) //Conexion fuera de la UPV
  const char Server_Host[] = "api.thingspeak.com";
  const int Server_HttpPort = 80;
#else
  const char Server_Host[] = "dweet.io";
  const int Server_HttpPort = 80;
#endif

WiFiClient client;

///////////////////////////////////////////////////////
/////////////// HTTP REST Connection ////////////////
//////////////////////////////////////////////////////

#ifdef REST_SERVER_THINGSPEAK 
  const char Rest_Host[] = "api.thingspeak.com";
  String MyWriteAPIKey="53NUJ2ME5T7NMVFL"; // clave de canal ThingSpeak
#else 
  const char Rest_Host[] = "dweet.io";
  String MyWriteAPIKey="cdiocurso2023g01"; // clave de canal Dweet
#endif

//Numero de medidas a enviar al servidor REST (Entre 1 y 8)
#define NUM_FIELDS_TO_SEND 5 

#define power_PIN 5
#define channelTemp 1
#define channelHumedad 0
#define channelPh 2
#define channelLum 3
//#define channelSal 2

int16_t humedadValue = 0;
int16_t mediaHumedad;
int16_t tempValue = 0;
float Offset = -0.20;
char dato=' ';

Adafruit_ADS1115 ads1115;

SoftwareSerial gps(12,13);

/***************************************
 *                                     *
 *                 *                   *
 *                * *                  *
 *               * * *                 *
 *              * * * *                *
 *             * * * * *               *
 *            * * * * * *              *
 *           CODIGO  FINAL             *
 *           * * * * * * *             *
 *               |||||                 *
 ***************************************/

void setup() {
  #ifdef PRINT_DEBUG_MESSAGES
    Serial.begin(9600);
  #endif
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

  
  
  connectWiFi();

  #ifdef PRINT_DEBUG_MESSAGES
      Serial.print("Server_Host: ");
      Serial.println(Server_Host);
      Serial.print("Port: ");
      Serial.println(String( Server_HttpPort ));
      Serial.print("Server_Rest: ");
      Serial.println(Rest_Host);
  #endif

  gps.begin(9600); 
  Serial.println("Inicializando el GPS...");
  delay(27000);
  Serial.println("Esperando datos");
}

void loop() {
  Serial.println("*********************");
  
  String data[ NUM_FIELDS_TO_SEND + 1];

  data[1] = medirHumedad(channelHumedad);

  Serial.println("------");

  data[2] = medirTemperatura(channelTemp);

  Serial.println("------");
  
  data[3] = medirPh(channelPh);

  Serial.println("------");
  
  data[4] = medirLum(channelLum);

  Serial.println("------");
  
  data[5] = medirSalinidad();

  Serial.println("------");

  HTTPGet(data, NUM_FIELDS_TO_SEND);

  gps();
  
  delay(5000);
}

int medirHumedad(int channelValue) {  
  humedadValue = ads1115.readADC_SingleEnded(channelValue);
  int humedadPorcentaje = map(humedadValue, 30200, 16800, 0, 100);
  
  Serial.print("Humedad: ");
  Serial.println(humedadValue,DEC);
  Serial.print("Porcentaje de humedad: ");
  Serial.print(humedadPorcentaje,DEC);
  Serial.println("%");

  return humedadPorcentaje;
}

float medirTemperatura (int channelValue) {
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

  return temperatura;
}

float medirSalinidad () {
  int16_t adc0;

  digitalWrite(power_PIN, HIGH);
  delay(100);
  
  adc0 = analogRead(A0);
  digitalWrite(power_PIN, LOW);
  delay(100);

  Serial.print("Lectura sal  = ");
  Serial.println(adc0, DEC);

//
//  float grSal = (adc0 - 575.6) / 6.32;
//
//  Serial.print("Gramos de sal: ");
//  Serial.println(grSal);
//
//  return grSal;
}

float medirPh (int channelValue) {
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

  return pHValue;
}

int16_t medirLum (int channelValue) {
  int16_t adc0 = ads1115.readADC_SingleEnded(channelValue);

  Serial.println(adc0);
  if (adc0 <= 400 ) {
    Serial.println("Luminosidad: oscuro");
  } else if (adc0 <= 1100 ) {
    Serial.println("Luminosidad: sombra");
  } else if (adc0 <= 4000 ) {
    Serial.println("Luminosidad: luz ambiente");
  } else if (adc0 <=30200 ) {
    Serial.println("Luminosidad: luz de movil");
  } 

  return adc0;
}

int16_t averageSample (int ArrayLength, int channelValue) {
  int16_t media = 0;
  for (int i = 0; i < ArrayLength; i++) {
      media += ads1115.readADC_SingleEnded(channelValue);
  }
  
  return media;
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

void gps() {
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
