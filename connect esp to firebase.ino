#include <ArduinoJson.h>
#include <SoftwareSerial.h>

// Allocate the JSON document
// This one must be bigger than for the sender because it must store the strings
StaticJsonDocument<300> doc;

// Declare the "link" serial port
// Please see SoftwareSerial library for detail
SoftwareSerial linkSerial(GPC15, GPC1); // RX, TX de la carte esp8266 Node

void setup() {
  // Initialize "debug" serial port
  // The data rate must be much higher than the "link" serial port
  Serial.begin(9600);
  // Initialize the "link" serial port
  // Use the lowest possible data rate to reduce error ratio
  linkSerial.begin(4800);
}
 
void loop() {
  // Check if the other Arduino is transmitting
  if (linkSerial.available()) 
  {
    // Read the JSON document from the "link" serial port
    deserializeJson(doc, linkSerial);
    int val1 = doc["cap1"].as<int>();
    int val2 = doc["cap2"].as<int>();
    Serial.println(val1);
    Serial.println(val2);

  }
}

