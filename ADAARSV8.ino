/*
  Author  : Jayakrishnan K
  Handle  : JK
  Version : 7.0
  Date    : 26-3-2021
  Time    : 9:20 PM IST
  Project : Accident Data Analysis Aquistition and Recovery System
  College : GEC Thrissur
  Concept : Proof of Principle
*/

#include <Wire.h>
#include <MPU6050.h>
#include <Servo.h>

MPU6050 mpu;    // Create MPU object to control a servo
Servo myservo;  // Create servo object to control a servo

// Timers
unsigned long GCST = 0;   // Groove Coupler Count Start Time Instant
unsigned long GCET = 0;   // Groove Coupler Count End Time Instant
unsigned long IST = 0;   // Post Impact Time Count Start Time Instant
unsigned long IET = 0;   // Post Impact Time Count End Time Instant
unsigned long timer = 0;   // Timer to pass the complete processing time
int pos;    // Position of the Servo shaft
int steps=0;    // Steps count in Sensor Groove Coupler
float steps_old=0;    // Previous Steps count in Sensor Groove Coupler
float temp=0;   // Temperory variable to store the RPS
float rps=0;    // Variable to store the RPS
float timeStep = 0.01;    // Constant to update orientation and calculate delay
float SWA =0;   // Steering Wheel Angle
float pitch = 0;    // Pitch
float roll = 0;    // Roll
float yaw = 0;    // Yaw
long duration; // Duration of sound wave travel
int distance; // Distance measurement
int counter = 0;    // Counter of the Steps in Rotary Encoder
int count=0;    
int aState;   // Current state in Rotary Encoder
int aLastState;   // Last state in Rotary Encoder
int GRV=0;    // Reverse Gear Indication
int G1V=0;    // First Gear Indication
int G2V=0;    // Second Gear Indication
int G3V=0;    // Third Gear Indication
int G=0;    //  Gear Position Indication
int Impact = 0;   // Impact indication

#define echoPin 2   // Pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3   // Pin D3 Arduino to pin Trigger of HC-SR04
#define impactPin 4   // Pin D4 Arduino to Imapct switch
#define GC 5    // Pin D5 Arduino to Groove Coupler D0 pin
#define outputA 6  // Pin D6 Arduino to Rotary Encoder CLK pin
#define outputB 7  // Pin D7 Arduino to Rotary Encoder DATA pin
#define GR 10   // Pin D10 Reverse Gear pin of Rotary switch ,Violet wire
#define G1 11  // Pin D11 First Gear pin of Rotary switch ,Yellow wire
#define G2 12   // Pin D12 Second Gear pin of Rotary switch ,Brown wire
#define G3 13   // Pin D13 Third Gear pin of Rotary switch ,Blue wire

void setup() 
{
  pinMode (outputA,INPUT);    // Sets the CLK pin as an Input
  pinMode (outputB,INPUT);    // Sets the DATA pin as an Input
  pinMode(trigPin, OUTPUT);   // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);    // Sets the echoPin as an Input
  pinMode(impactPin,INPUT);   // Sets the Impact pin as an Input
  pinMode(GC,INPUT);    // Sets the Groove Coupler D0 pin as an Input
  // Sets the four Rotary switch pins as Inputs
  pinMode(GR,INPUT);    
  pinMode(G1,INPUT);
  pinMode(G2,INPUT);
  pinMode(G3,INPUT);
  
  myservo.attach(9);  // Attaches the servo on pin 9 to the servo object
  
  Serial.begin(9600);   // Serial Transmission Begining
  // MPU Existence Check
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  aLastState = digitalRead(outputA);    // Reads the initial state of the outputA

  // Calibrate gyroscope. The calibration must be at rest.
  // If you don't want calibrate, comment this line.
  mpu.calibrateGyro();

  // Set threshold sensivty. Default 3.
  // If you don't want use threshold, comment this line or set 0.
  mpu.setThreshold(3);
  // Check settings
 // checkSettings();

  Serial.println("CLEARDATA");    // Clear the Excel data
  Serial.println("LABEL,Date,Time,X,Y,Z,Pitch,Roll,Yaw,Temperature,SWA,BPD(cm),Speed(Hz),Gear,Impact");   // Headings of Columns in Excel
  Serial.println("RESETTIMER");   // PLX DAQ Timer reset
}

// Function for Checking settings of MPU
/*void checkSettings()
{
  Serial.println();
  
  Serial.print(" * Sleep Mode:        ");
  Serial.println(mpu.getSleepEnabled() ? "Enabled" : "Disabled");
  
  Serial.print(" * Clock Source:      ");
  switch(mpu.getClockSource())
  {
    case MPU6050_CLOCK_KEEP_RESET:     Serial.println("Stops the clock and keeps the timing generator in reset"); break;
    case MPU6050_CLOCK_EXTERNAL_19MHZ: Serial.println("PLL with external 19.2MHz reference"); break;
    case MPU6050_CLOCK_EXTERNAL_32KHZ: Serial.println("PLL with external 32.768kHz reference"); break;
    case MPU6050_CLOCK_PLL_ZGYRO:      Serial.println("PLL with Z axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_YGYRO:      Serial.println("PLL with Y axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_XGYRO:      Serial.println("PLL with X axis gyroscope reference"); break;
    case MPU6050_CLOCK_INTERNAL_8MHZ:  Serial.println("Internal 8MHz oscillator"); break;
  }
  
  Serial.print(" * Gyroscope:         ");
  switch(mpu.getScale())
  {
    case MPU6050_SCALE_2000DPS:        Serial.println("2000 dps"); break;
    case MPU6050_SCALE_1000DPS:        Serial.println("1000 dps"); break;
    case MPU6050_SCALE_500DPS:         Serial.println("500 dps"); break;
    case MPU6050_SCALE_250DPS:         Serial.println("250 dps"); break;
  } 
  
  Serial.print(" * Gyroscope offsets: ");
  Serial.print(mpu.getGyroOffsetX());
  Serial.print(" / ");
  Serial.print(mpu.getGyroOffsetY());
  Serial.print(" / ");
  Serial.println(mpu.getGyroOffsetZ());
  
  Serial.println();
}

  // If you want, you can set accelerometer offsets
  // mpu.setAccelOffsetX();
  // mpu.setAccelOffsetY();
  // mpu.setAccelOffsetZ();*/


void loop()
{
  Impact = digitalRead(impactPin);    // Read the Impact pin state
  
  // *Code for Groove Coupler*
  GCST=millis();    // Groove Coupler Step counting Start Time to this instant
  GCET=GCST+1000;   // Groove Coupler Step counting End Time after 1 second(1000 ms) start time
   // count the steps till 1 second
   while(millis()<GCET)
    {
     if(digitalRead(GC))
    {
       steps=steps+1; 
       while(digitalRead(GC));
     }
    }
    
    temp=steps-steps_old;   //Get number of rotations
    steps_old=steps;    // Assigns the current number of steps to previous number of steps
    rps=temp;   // Get the Rotation Per Second
    
  // *Code for USD*
  digitalWrite(trigPin, LOW);   // Clears the trigPin condition
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);    // Reads the echoPin, returns the sound wave travel time in microseconds
  distance = duration * 0.034 / 2; // Calculating the distance, Speed of sound wave divided by 2 (go and back)
  
  count=count+ 1;   // Count the number of times in which the loop executed
  timer = millis();   // timer value to this instant
  
  // MPU read normalized values
  Vector norm = mpu.readNormalizeGyro();
  Vector rawAccel = mpu.readRawAccel();
  Vector normAccel = mpu.readNormalizeAccel();
  float temp = mpu.readTemperature();

  // Calculate Pitch, Roll and Yaw of MPU
  pitch = pitch + norm.YAxis * timeStep;
  roll = roll + norm.XAxis * timeStep;
  yaw = yaw + norm.ZAxis * timeStep;

  // *Code for Rotary Encoder*
  aState = digitalRead(outputA); // Reads the "current" state of the outputA
   // If the previous and the current state of the outputA are different, that means a Pulse has occured
   if (aState != aLastState){     
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
     if (digitalRead(outputB) != aState) { 
       counter ++;
     } else {
       counter --;
     }
     pos = counter*-4.5+90;   // Calculating Servo motor shaft angle
     SWA= counter*9;    // Calculation of Steering wheel angle
     myservo.write(pos);    // Turning shaft of Servo motor to corresponding position of Steering wheel
   } 
  aLastState = aState; // Updates the previous state of the outputA with the current state
  
  // *Code for Gear Position
  // Read the Gear position pin states
  GRV = digitalRead(GR);    
  G1V = digitalRead(G1);
  G2V = digitalRead(G2);
  G3V = digitalRead(G3);

  // Assigning G the values corresponding to gear position
  if(GRV ==1){
    G=0;
  }
  else if(G1V ==1){
    G=1;
  }
  else if(G2V ==1){
    G=2;
  }
  else if(G3V ==1){
    G=3;
  }

  // Printing the values in Excel by PLX DAQ
  Serial.print("DATA,DATE,TIME,");
  Serial.print(normAccel.XAxis);
  Serial.print(",");
  Serial.print(normAccel.YAxis);
  Serial.print(",");
  Serial.print(normAccel.ZAxis);
  Serial.print(",");
  Serial.print(pitch);
  Serial.print(",");
  Serial.print(roll);
  Serial.print(",");
  Serial.print(yaw);
  Serial.print(",");
  Serial.print(temp);
  Serial.print(",");
  Serial.print(SWA);
  Serial.print(",");
  Serial.print(distance);
  Serial.print(",");
  Serial.print(rps);
  Serial.print(",");
  Serial.print(G);
  Serial.print(",");
  Serial.println(Impact);
  
  delay((timeStep*10000) - (millis() - timer));   // Delay to confirm the data is  fetched

  // * Code for response in impact*
  Impact = digitalRead(impactPin);
  //    If no impact print 20 rows and clear it and repeat
  if(Impact == 0){
    if(count>20) {
      count=0;
      Serial.println("CLEARDATA");
    }
  }
  //    If impact print 10 rows, and end serial data transmission
   else if(Impact == 1){
    count=0;
    if(count>10) {
      Serial.end();  

      }
    }
}
