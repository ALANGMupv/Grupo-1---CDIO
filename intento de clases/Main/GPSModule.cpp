// GPSModule.cpp

#include "GPSModule.h"

GPSModule::GPSModule(SoftwareSerial& gpsSerial) : gpsSerial(gpsSerial) {}

void GPSModule::readGPSData() {
    if (gpsSerial.available()) {
        String nmeaSentence = gpsSerial.readStringUntil('\n');

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
