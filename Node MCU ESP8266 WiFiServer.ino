// Code for sending all data and viewing all data from MPU and Impact Sensor in one at same page

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <MPU6050.h>
const char *ssid = "ADAARS";
const char *password = "12345678";
WiFiServer server(80);
MPU6050 mpu;
float timeStep = 0.01;
float pitch = 0;    // Pitch
float roll = 0;    // Roll
float yaw = 0;    // Yaw
float X_Acc = 0;
float Y_Acc = 0;
float Z_Acc = 0;
float T =0;
bool Impact = false;
#define impactPin D5

void setup() {
pinMode(impactPin,INPUT);
WiFi.softAP(ssid,password);
Serial.begin(9600);
server.begin();
while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
}
void loop() {
Impact = digitalRead(impactPin);

  Vector norm = mpu.readNormalizeGyro();
  Vector rawAccel = mpu.readRawAccel();
  Vector normAccel = mpu.readNormalizeAccel();
  T = mpu.readTemperature();
  X_Acc = normAccel.XAxis;
  Y_Acc = normAccel.YAxis;
  Z_Acc = normAccel.ZAxis;

  pitch = pitch + norm.YAxis * timeStep;
  roll = roll + norm.XAxis * timeStep;
  yaw = yaw + norm.ZAxis * timeStep;
  
WiFiClient client = server.available();   // Listen for incoming clients

client.println("Pitch= "+String(pitch));
client.println("Roll= " +String(roll));
client.println("Yaw= "+String(yaw));
client.println("X= "+String(X_Acc));
client.println("Y= "+String(Y_Acc));
client.println("Z= "+String(Z_Acc));
client.println("Impact= "+String(Impact));
client.println("Temperature= "+String(T));
Serial.println();
Serial.println("Pitch= "+String(pitch));
Serial.println("Roll= " +String(roll));
Serial.println("Yaw= "+String(yaw));
Serial.println("X= "+String(X_Acc));
Serial.println("Y= "+String(Y_Acc));
Serial.println("Z= "+String(Z_Acc));
Serial.println("Impact= "+String(Impact));
Serial.println("Temperature= "+String(T));

//delay(600);
}
