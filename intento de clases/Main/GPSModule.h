// GPSModule.h

#ifndef GPSModule_h
#define GPSModule_h

#include <SoftwareSerial.h>

class GPSModule {
public:
    GPSModule(SoftwareSerial& gpsSerial);
    void readGPSData();

private:
    SoftwareSerial& gpsSerial;
};

#endif