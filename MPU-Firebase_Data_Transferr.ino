
/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <MPU6050.h> 
/* Set these to your desired credentials. */
#define FIREBASE_HOST  "adaars-1eb92-default-rtdb.firebaseio.com" //Without http:// or https:// schemes
#define FIREBASE_AUTH "C2S9CMsgQJwCGCXO5WilinHN9toCbXxu0G9VRo7b"
#define WIFI_SSID "DEM"
#define WIFI_PASSWORD "qwertyas"
MPU6050 mpu;
float timeStep = 0.01;
float Pitch=0;    // Pitch
float Roll = 0;    // Roll
float Yaw = 0;    // Yaw
float X_Acc = 0;
float Y_Acc = 0;
float Z_Acc = 0;
float TMP =0;
int IMP = 0;
#define impactPin D5

FirebaseData ADAARS;

FirebaseJson json;

void setup() {
  pinMode(impactPin,INPUT_PULLUP);
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  

  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
}

void loop()
{
  IMP = not digitalRead(impactPin);
  Vector norm = mpu.readNormalizeGyro();
  Vector rawAccel = mpu.readRawAccel();
  Vector normAccel = mpu.readNormalizeAccel();
  TMP = mpu.readTemperature();
  X_Acc = normAccel.XAxis;
  Y_Acc = normAccel.YAxis;
  Z_Acc = normAccel.ZAxis;

  Pitch = Pitch + norm.YAxis * timeStep;
  Roll = Roll + norm.XAxis * timeStep;
  Yaw = Yaw + norm.ZAxis * timeStep;

  Firebase.setInt(ADAARS, "/RR2006/IMP", IMP);
  Firebase.setFloat(ADAARS, "/RR2006/TMP", TMP);
  Firebase.setFloat(ADAARS, "/RR2006/X_Acc", X_Acc);
  Firebase.setFloat(ADAARS, "/RR2006/Y_Acc", Y_Acc);
  Firebase.setFloat(ADAARS, "/RR2006/Z_Acc", Z_Acc);
  Firebase.setFloat(ADAARS, "/RR2006/Pitch", Pitch);
  Firebase.setFloat(ADAARS, "/RR2006/Roll", Roll);
  Firebase.setFloat(ADAARS, "/RR2006/Yaw", Yaw);
  

  /*Serial.print(X_Acc);
  Serial.print(Y_Acc);
  Serial.print(Z_Acc);
  Serial.print(Pitch);
  Serial.print(Roll);
  Serial.print(Yaw);
  Serial.print(TMP);
  Serial.println(IMP);*/
}
