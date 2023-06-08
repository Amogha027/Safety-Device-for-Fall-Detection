#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <TinyGPSPlus.h>
#include <LoRa.h>
#include <SPI.h>

#define ss 5
#define rst 4
#define dio0 2

Adafruit_MPU6050 mpu;
TinyGPSPlus gps;

int count = 0, ID = 2;

void displayInfo(int flag)
{
    LoRa.beginPacket();
    Serial.print("ID: ");
    Serial.print(ID);
    LoRa.print(ID);
    LoRa.print(" ");
    
    Serial.print(" Fall: ");
    if (flag == 1) {
      Serial.println("Sending an E-mail");
      Serial.print(flag);
      LoRa.print(flag);
    }
    else {
      Serial.print(flag);
      LoRa.print(flag);
    }
    LoRa.print(" ");
      
    Serial.print(F(" Location: "));
  
    if (gps.location.isValid()) {
      Serial.print("Sending packet: ");
      Serial.print("Lat: ");
      Serial.print(gps.location.lat(), 6);
      Serial.print(F(", "));
      Serial.print("Lng: ");
      Serial.print(gps.location.lng(), 6);
      Serial.println();
  
      LoRa.print(gps.location.lat(), 6);
      LoRa.print((" "));
      LoRa.print(gps.location.lng(), 6);
      LoRa.println();
    }
    else {
      Serial.println(F("INVALID"));
    }
    LoRa.endPacket();
}  

void setup() {
    Serial.begin(9600);
    while (!Serial);
    Serial.println("LoRa Sender");
  
    Serial2.begin(9600);
    while (!Serial2);
    Serial.println("gps Sender");
  
    LoRa.setPins(ss, rst, dio0);
  
    while (!LoRa.begin(433E6))
    {
      Serial.println(".");
      delay(500);
    }
    LoRa.setSyncWord(0xB1);
    Serial.println("LoRa Initializing OK!");
  
    if (!mpu.begin()) {
      Serial.println("Failed to find MPU6050 chip");
      while (1) {
        delay(10);
      }
    }
    Serial.println("MPU6050 Found!");
  
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    Serial.print("Accelerometer range set to: ");
    switch (mpu.getAccelerometerRange()) {
      case MPU6050_RANGE_2_G:
        Serial.println("+-2G");
        break;
      case MPU6050_RANGE_4_G:
        Serial.println("+-4G");
        break;
      case MPU6050_RANGE_8_G:
        Serial.println("+-8G");
        break;
      case MPU6050_RANGE_16_G:
        Serial.println("+-16G");
        break;
    }
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    Serial.print("Gyro range set to: ");
    switch (mpu.getGyroRange()) {
      case MPU6050_RANGE_250_DEG:
        Serial.println("+- 250 deg/s");
        break;
      case MPU6050_RANGE_500_DEG:
        Serial.println("+- 500 deg/s");
        break;
      case MPU6050_RANGE_1000_DEG:
        Serial.println("+- 1000 deg/s");
        break;
      case MPU6050_RANGE_2000_DEG:
        Serial.println("+- 2000 deg/s");
        break;
    }
  
    mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
    Serial.print("Filter bandwidth set to: ");
    switch (mpu.getFilterBandwidth()) {
      case MPU6050_BAND_260_HZ:
        Serial.println("260 Hz");
        break;
      case MPU6050_BAND_184_HZ:
        Serial.println("184 Hz");
        break;
      case MPU6050_BAND_94_HZ:
        Serial.println("94 Hz");
        break;
      case MPU6050_BAND_44_HZ:
        Serial.println("44 Hz");
        break;
      case MPU6050_BAND_21_HZ:
        Serial.println("21 Hz");
        break;
      case MPU6050_BAND_10_HZ:
        Serial.println("10 Hz");
        break;
      case MPU6050_BAND_5_HZ:
        Serial.println("5 Hz");
        break;
    }
}

void loop() {
    // updateSerial();
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    float mean = pow(pow(a.acceleration.x, 2) + pow(a.acceleration.y, 2) + pow(a.acceleration.z, 2), 0.5);
    Serial.println(mean);
    if (mean < 1.5) {
      count++;
  
      if (count > 0) {
        while (Serial2.available() > 0)
          if (gps.encode(Serial2.read()))
            displayInfo(1);
  
        if (millis() > 5000 && gps.charsProcessed() < 10) {
          Serial.println(F("No GPS detected: check wiring."));
        }
      }
    }
    else {
      count = 0;
      while (Serial2.available() > 0)
          if (gps.encode(Serial2.read()))
            displayInfo(0);
  
       if (millis() > 5000 && gps.charsProcessed() < 10) {
         Serial.println(F("No GPS detected: check wiring."));
        }
    }
  
//    LoRa.beginPacket();
//    LoRa.print(mean);
//    LoRa.println();
//    LoRa.endPacket();
    delay(100);
}                    

void updateSerial() {
    delay(500);
    while (Serial.available()) {
      Serial2.write(Serial.read());  // Forward what Serial received to Software Serial Port
    }
    while (Serial2.available()) {
      Serial.write(Serial2.read());  // Forward what Software Serial received to Serial Port
    }
}
