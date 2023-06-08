#include <TinyGPSPlus.h>
#include <LoRa.h>
#include <SPI.h>

#define ss 5
#define rst 14
#define dio0 2

TinyGPSPlus gps;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("LoRa Sender");

  LoRa.setPins(ss, rst, dio0); 
  
  while (!LoRa.begin(433E6))     //433E6 - Asia, 866E6 - Europe, 915E6 - North America
  {
    Serial.println(".");
    delay(500);
  }
  LoRa.setSyncWord(0xB1);
  Serial.println("LoRa Initializing OK!");
  Serial2.begin(9600);
}

void loop() {
  //updateSerial();
  while (Serial2.available() > 0)
    if (gps.encode(Serial2.read()))
      displayInfo();
      
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }
}

void displayInfo()
{
  Serial.print(F("Location: "));

  if (gps.location.isValid()) {
    Serial.print("Lat: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(", "));
    Serial.print("Lng: ");
    Serial.print(gps.location.lng(), 6);
    Serial.println();

    LoRa.print("Sending packet: ");
    LoRa.beginPacket();
    LoRa.print("Lat: ");
    LoRa.print(gps.location.lat(), 6);
    LoRa.print(F(", "));
    LoRa.print("Lng: ");
    LoRa.print(gps.location.lng(), 6);
    LoRa.println();
    LoRa.endPacket();
  }  
  else {
    Serial.println(F("INVALID"));
  }
}

void updateSerial() {
  delay(500);
  while (Serial.available()) {
    Serial2.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (Serial2.available()) {
    Serial.write(Serial2.read());//Forward what Software Serial received to Serial Port
  }
}
