/**
 * Simple server compliant with Mozilla's proposed WoT API
 * Based on the RGBLamp example
 * Tested on Arduino Mega with Ethernet Shield
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <Arduino.h>
#include <stdio.h>
#include "Thing.h"
#include "WebThingAdapter.h"

int countator = 0;

//TODO: Hardcode your wifi credentials here (and keep it private)
const char* ssid = "Fastlan";
const char* password = "cicciottocartofier2";
WebThingAdapter* adapter;

const char* deviceTypes[] = {"MultiLevelSensor", "Sensor", nullptr};
ThingDevice device("SensDevice", "Analog Sensor pluged in single pin", deviceTypes);
ThingProperty property("level", "Analog Input pin", NUMBER, "LevelProperty");

//WebThingAdapter* adapter = NULL;

const int sensorPin = A0;

double lastValue = 0;
/*
int setupNetwork() {
  Serial.println(__FUNCTION__);
  //TODO: update with actual MAC address
  uint8_t ETHERNET_MAC[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  uint8_t error = Ethernet.begin(ETHERNET_MAC);
  if (error == 0)
  {
    printf("error: %s\n",__FUNCTION__);
    return -1;
  }
  return 0;
}
*/
const int ledPin = 2;
void setup(void) {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  
  Serial.begin(115200);
  Serial.println(__FUNCTION__);
  Serial.println("");
  Serial.print("Connecting to \"");
  Serial.print(ssid);
  Serial.println("\"");
#if defined(ESP8266) || defined(ESP32)
  WiFi.mode(WIFI_STA);
#endif
  WiFi.begin(ssid, password);
  Serial.println("");
  // Wait for connection
  bool blink = true;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(ledPin, blink ? LOW : HIGH); // active low led
    blink = !blink;
  }
  /*IPAddress ip = Ethernet.localIP();
  Serial.print("log: IP=");
  Serial.println(ip);
  delay(3000);*/
  digitalWrite(ledPin, HIGH);
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  adapter = new WebThingAdapter("w27", WiFi.localIP());
  adapter = new WebThingAdapter("analog-sensor", WiFi.localIP());
  device.addProperty(&property);
  adapter->addDevice(&device);
  Serial.println("Starting HTTP server");
  adapter->begin();
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(device.id);
}

void loop(void) {
  /*const int threshold = 1;
  int value = analogRead(sensorPin);
  double percent = (double) 100. - (value/1204.*100.);
  if (abs(percent - lastValue) >= threshold) {
    Serial.print("log: Value: ");
    Serial.print(value);
    Serial.print(" = ");
    Serial.print(percent);
    Serial.println("%");
    ThingPropertyValue levelValue;
    levelValue.number = percent;
    property.setValue(levelValue);
    lastValue = percent;
  }*/
  
  countator++;
  if (countator >= 3000){
    countator = 0;  
    ThingPropertyValue levelValue;
    levelValue.number = random(0,100);
    property.setValue(levelValue);
    adapter->update();
  }
}
