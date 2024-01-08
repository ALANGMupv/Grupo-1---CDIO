#ifndef HTTPCommunication_h
#define HTTPCommunication_h

#include <ESP8266WiFi.h>

class HTTPCommunication {
public:
    HTTPCommunication(const char* serverHost, int serverPort);
    void HTTPGet(String fieldData[], int numFields);
    void HTTPPost(String fieldData[], int numFields);
    void setRestHost(const char* host);
    void setWriteAPIKey(const String& apiKey);
   
private:
    const char* serverHost;
    int serverPort;
    String restHost;
    String writeAPIKey;
    WiFiClient client;
};

#endif
