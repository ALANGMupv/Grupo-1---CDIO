#ifndef WiFiConnection_h
#define WiFiConnection_h

#include <ESP8266WiFi.h>

class WiFiConnection {
public:
    WiFiConnection(const char* ssid, const char* password);
    void connect();
    IPAddress getIPAddress();

private:
    const char* ssid;
    const char* password;
};

#endif
