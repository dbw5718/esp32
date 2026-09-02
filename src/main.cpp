#include <WiFi.h>
#include<WiFiManager.h>

WiFiManager wm;

void setup()
{
    Serial.begin(115200);
    bool res=wm.autoConnect("ESP32_AP");
    if(!res){
        Serial.println("Failed to connect");
        ESP.restart();
    }else{
        Serial.println("Connected :)");
        Serial.println("IP Address: ");
        Serial.println(WiFi.localIP());
    }

}

void loop()
{

}