#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <LoRa.h>
#include <SPI.h>

// sender code with mpu6050
#define ss 5
#define rst 14
#define dio0 2
 
int counter = 0;
Adafruit_MPU6050 mpu;
 
void setup() 
{
  Serial.begin(9600); 
  while (!Serial);
  Serial.println("LoRa Sender");
 
  LoRa.setPins(ss, rst, dio0);    //setup LoRa transceiver module
  
  while (!LoRa.begin(433E6))     //433E6 - Asia, 866E6 - Europe, 915E6 - North America
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
 
void loop() 
{
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  float mean = pow(pow(a.acceleration.x,2)+pow(a.acceleration.y,2)+pow(a.acceleration.z,2),0.5);
  Serial.println(mean);
//  Serial.print("Sending packet: ");
//  Serial.print("Acceleration X: ");
//  Serial.print(a.acceleration.x);
//  Serial.print(", Y: ");
//  Serial.print(a.acceleration.y);
//  Serial.print(", Z: ");
//  Serial.print(a.acceleration.z);
//  Serial.println(" m/s^2");
//
//  Serial.print("Rotation X: ");
//  Serial.print(g.gyro.x);
//  Serial.print(", Y: ");
//  Serial.print(g.gyro.y);
//  Serial.print(", Z: ");
//  Serial.print(g.gyro.z);
//  Serial.println(" rad/s");
//
//  Serial.print("Temperature: ");
//  Serial.print(temp.temperature);
//  Serial.println(" degC");
// 
  LoRa.beginPacket();   //Send LoRa packet to receiver
  LoRa.println(mean);
//  LoRa.print("Acceleration X: ");
//  LoRa.print(a.acceleration.x);
//  LoRa.print(", Y: ");
//  LoRa.print(a.acceleration.y);
//  LoRa.print(", Z: ");
//  LoRa.print(a.acceleration.z);
//  LoRa.println(" m/s^2");
//
//  LoRa.print("Rotation X: ");
//  LoRa.print(g.gyro.x);
//  LoRa.print(", Y: ");
//  LoRa.print(g.gyro.y);
//  LoRa.print(", Z: ");
//  LoRa.print(g.gyro.z);
//  LoRa.println(" rad/s");
//
//  LoRa.print("Temperature: ");
//  LoRa.print(temp.temperature);
//  LoRa.println(" degC");
  LoRa.endPacket();
 
 
  delay(1000);
}
