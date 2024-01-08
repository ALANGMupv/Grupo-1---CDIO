// HTTPCommunication.cpp

#include "HTTPCommunication.h"

HTTPCommunication::HTTPCommunication(const char* serverHost, int serverPort) {
    this->serverHost = serverHost;
    this->serverPort = serverPort;
}

void HTTPCommunication::HTTPGet(String data[], int numFields) {
    if (client.connect(serverHost, serverPort)) {
        String postData = "GET http://" + String(serverHost) + "/update?api_key=";
        
        for (int field = 0; field < numFields; field++) {
            postData += "&field" + String(field + 1) + "=" + data[field];
        }

        client.print(postData);
        client.println(" HTTP/1.1");
        client.println("Host: " + String(serverHost));
        client.println("Connection: close");
        client.println();

        delay(500);

        while (client.available()) {
            String line = client.readStringUntil('\r');
            Serial.print(line);
        }

        Serial.println();
        Serial.println();

        client.stop();
    }

    void HTTPCommunication::setRestHost(const char* host) {
      restHost = host;
    }

    void HTTPCommunication::setWriteAPIKey(const String& apiKey) {
      writeAPIKey = apiKey;
    }
}
