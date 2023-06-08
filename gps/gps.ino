#include <TinyGPSPlus.h>

// The TinyGPSPlus object
TinyGPSPlus gps;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  delay(3000);
}

void loop() {
  updateSerial();
  while (Serial2.available()) {
    Serial.println("lol");
    if (gps.encode(Serial2.read()))
      displayInfo();
  }
      
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
//    while (true);
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
  }  
  else {
    Serial.println(F("INVALID"));
  }
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) {
    Serial2.write(Serial.read());//Forward what Serial received to Software Serial Port
    Serial.println("one");
  }
  while (Serial2.available()) {
    Serial.write(Serial2.read());//Forward what Software Serial received to Serial Port
    Serial.println("two");
  }
}
