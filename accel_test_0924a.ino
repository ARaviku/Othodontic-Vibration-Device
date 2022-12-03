#include <Wire.h> //load library 
int16_t raw_AccX,raw_AccY,raw_AccZ; //Create 16-bit integer type variables for raw accelerations
float AccX,AccY,AccZ,Mag; //Create float type variables for accelerations
double currTime;

void setup() // put your setup code here, to run once:
{
  Wire.begin(); //Initialize connection
  Wire.beginTransmission(0x68); //Transmit to MPU address
  Wire.write(0x6B); //Write to power management register
  Wire.write(0); //Send value of 0 to wake up MPU
  Wire.endTransmission(true); //release I2C bus
  Serial.begin(9600); //Initialize serial monitor
}

void loop() // put your main code here, to run repeatedly:
{
  Wire.beginTransmission(0x68); //Transmit to MPU address
  Wire.write(0x3B); //Write to first x acceleration register
  Wire.endTransmission(false); //Maintain connection
  Wire.requestFrom(0x68,6,true); //Read next 6 registers, starting from 0x3B
  currTime = millis();
  raw_AccX = Wire.read()<<8|Wire.read(); //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L) 
  raw_AccY = Wire.read()<<8|Wire.read(); //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  raw_AccZ = Wire.read()<<8|Wire.read(); //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  AccX = raw_AccX/16384.0; //Convert from raw value to g's
  AccY = raw_AccY/16384.0; //Convert from raw value to g's
  AccZ = raw_AccZ/16384.0; //Convert from raw value to g's
  Mag = (abs(AccX)+abs(AccY)+abs(AccZ))/3;
  Serial.print(currTime);Serial.print("\t");
  Serial.print(AccX);Serial.print("\t"); //Output x acceleration
  Serial.print(AccY);Serial.print("\t"); //Output y acceleration
  Serial.print(AccZ);Serial.println("\t"); //Output z acceleration
  delay(50);
}
