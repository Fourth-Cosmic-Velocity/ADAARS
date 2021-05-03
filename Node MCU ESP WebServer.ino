/*Code for posting the MPU data and Impact sensor data on server as subdirectory and a device can see those data by connecting to this WiFi AP made with the help of Mr.Jinson V Shaju
/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <MPU6050.h>
/* Set these to your desired credentials. */
const char *ssid = "ADAARS";
const char *password = "qwertyas";

ESP8266WebServer server(80);
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

//Go to http://192.168.4.1 in a web browser
//connected to this access point to see it.
void X()
{
  server.send(200, "text/plain", String(X_Acc));
}

void Y()
{
  server.send(200, "text/plain", String(Y_Acc));
}

void Z()
{
  server.send(200, "text/plain", String(Z_Acc));
}

void Pitch() 
{
  server.send(200, "text/plain",String(pitch));
}

void Roll() 
{
  server.send(200, "text/plain",String(roll));
}

void Yaw() 
{
  server.send(200, "text/plain",String(yaw));
}

void TMP() 
{
  server.send(200, "text/plain",String(T));
}


void IMP() 
{
  server.send(200, "text/plain",String(Impact));
}

void handleNotFound()
{
   server.send(200, "text/plain","SORRY" );
}


void setup() {
  pinMode(impactPin,INPUT);
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  
  
  server.on("/X",X);
  server.on("/Y",Y);
  server.on("/T",TMP);
  server.on("/I",IMP);
  server.on("/P",Pitch);
  server.on("/R",Roll);
  server.on("/W",Yaw);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
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
  server.handleClient();
  //delay(500);
}
