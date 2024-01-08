#include "WiFiConnection.h"

WiFiConnection::WiFiConnection(const char* ssid, const char* password) {
    this->ssid = ssid;
    this->password = password;
}

void WiFiConnection::connect() {
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("Conectando wifi...");
        delay(500);
    }
}

IPAddress WiFiConnection::getIPAddress() {
    return WiFi.localIP();
}
