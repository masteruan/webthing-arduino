/**
 * Simple server compliant with Mozilla's proposed WoT API
 * Originally based on the HelloServer example by Mozilla Wot.
 * Giovanni Gentile - projectj.it
 * aka masteruan
 * 25 January 2019
 * Tested on EPS8266-01 with relay modules.
 * You can buy the board by using: https://amzn.to/2B3hiWy
 *
 * This Source Code is under CC BY-NC-SA 3.0
 */

#include <Arduino.h>
#include "Thing.h"
#include "WebThingAdapter.h"
#include <ESP8266WiFi.h>
#include "SoftwareSerial.h"

byte relON[] = {0xA0, 0x01, 0x01, 0xA2};  //Hex command to send to serial for open relay
byte relOFF[] = {0xA0, 0x01, 0x00, 0xA1}; //Hex command to send to serial for close relay

//TODO: Hardcode your wifi credentials here (and keep it private)
const char* ssid = "SSID";
const char* password = "";

WebThingAdapter* adapter;
SoftwareSerial esp8266(0,1);

const char* ledTypes[] = {"RelaySwitch", "Light", nullptr};
ThingDevice led("relay", "Relay-board", ledTypes);
ThingProperty ledOn("on", "", BOOLEAN, "OnOffProperty");

bool lastOn = false;

void setup(void){
  
  esp8266.begin(9600);
  
#if defined(ESP8266) || defined(ESP32)
  WiFi.mode(WIFI_STA);
#endif
  WiFi.begin(ssid, password);
  //Serial.println("");

  // Wait for connection
  bool blink = true;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  adapter = new WebThingAdapter("w26", WiFi.localIP());

  led.addProperty(&ledOn);
  adapter->addDevice(&led);
  adapter->begin();
}

void loop(void){
  adapter->update();
  bool acceso = ledOn.getValue().boolean;
  if (acceso != lastOn) {
    if (acceso) { 
      esp8266.write(relON, sizeof(relON));
      delay(10);
      esp8266.write(relON, sizeof(relON));
    }
    else{
      esp8266.write(relOFF, sizeof(relOFF));
      delay(10);
      esp8266.write(relOFF, sizeof(relOFF));
    }
  }
  lastOn = acceso;
}
