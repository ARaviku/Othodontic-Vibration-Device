// Define FHT parameters.
#define LIN_OUT 1 // use the lin output function
#define FHT_N 256 // set to 256 point fht
// =================================================================================

// Libraries.
#include <FHT.h>
#include <Wire.h> 
#include <SparkFun_MMA8452Q.h>
// =================================================================================

// Create instance of MMA8452Q accelerometer.
MMA8452Q accel;
// =================================================================================

// Global variables for frequency calculations.
double freq[3]; //array to hold frequency of x,y,z
double time1; //timing variable before measurement
double time2; //timing variable after measurement
double time_diff; //(time2-time1)/FHT_N : sampling period
double delta_f; //(1000/time_diff)/FHT_N : frequency bin
double desired_freq;
double current_freq;
// =================================================================================

// Global variables for PID controller.
double error;
double kp = 0.5;
double PID;
// =================================================================================

// Define input pins for H-bridge motor driver.
#define PWM1 5 // pin 1
#define PWM2 6 // pin 9
#define left1 7 // pin 2
#define left0 8 // pin 7
#define right1 9 // pin 15
#define right0 10 // pin 10

// Initialize control and power variables.
int control = 0;
int input = 0;
// =================================================================================

// Setup code.
void setup() {
  Serial.begin(115200); // use the serial port
  accel.init(); //initialize accelerometer (800Hz,+-2g)

    // Setup Arduino pins
  pinMode(A0, INPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(left1, OUTPUT);
  pinMode(left0, OUTPUT);
  pinMode(right1, OUTPUT);
  pinMode(right0, OUTPUT);

  // Setup directions of motor
  digitalWrite(left1, HIGH);
  digitalWrite(left0, LOW);
  digitalWrite(right1, HIGH);
  digitalWrite(right0, LOW);
}
// =================================================================================

// Loop code.
void loop() {

  // Find max frequency in x,y,and z 
  freq[0] = xFFT(); //get max freq index in X
  freq[1] = yFFT(); //get max freq index in y
  freq[2] = zFFT(); //get max freq index in z
  
  delta_f = (1000/time_diff)/FHT_N; //calculate freq bin
  
  freq[0] = (freq[0]*delta_f); //calculate max freq in x
  freq[1] = (freq[1]*delta_f); //calculate max freq in y
  freq[2] = (freq[2]*delta_f); //calculate max freq in z

  current_freq = (freq[0] + freq[1] + freq[2])/3;
  control = analogRead(A0);
  desired_freq = map(control,0,1023,50,100);
  input = map(desired_freq,50,100,127,255);
  motorPID();
  analogWrite(PWM1,input);
  analogWrite(PWM2,input);
  serialDisplay();
}
// =================================================================================

// Motor controller function.
void motorPID()
{
  error = desired_freq - current_freq;
  PID = kp*error;

  input = input + PID;
}
// =================================================================================

// Display values in serial port function.
void serialDisplay()
{
  Serial.print(desired_freq);Serial.print("\t");
  Serial.print(current_freq);Serial.println("\t");
}
// =================================================================================

// Calculate x-axis frequency function.
double xFFT()
{
  time1 = millis();
  for (int i = 0 ; i < FHT_N ; i++) 
  { // save 256 samples
    if (accel.available())
    {
      accel.read();
      fht_input[i] = accel.x; // put real data into bins
      delay(1);
      //Serial.println(millis());
    }
  }
  time2 = millis();
  time_diff = (time2-time1)/256;
  fht_reorder(); // reorder the data before doing the fht
  fht_run(); // process the data in the fht
  fht_mag_lin(); // take the output of the fht
  int maxWhenX=0;
  int maxVal=0;
  for (int i = 1 ; i < FHT_N/2 ; i++)
  {
    if (fht_lin_out[i] > maxVal)
    {
      maxVal = fht_lin_out[i];
      maxWhenX = i;
    }
  }
  return maxWhenX;
}
// =================================================================================

// Calculate y-axis frequency function.
double yFFT()
{
  for (int j = 0 ; j < FHT_N ; j++) 
  { // save 256 samples
    if (accel.available())
    {
      accel.read();
      fht_input[j] = accel.y; // put real data into bins
      delay(1);
    }
  }
  fht_reorder(); // reorder the data before doing the fht
  fht_run(); // process the data in the fht
  fht_mag_lin(); // take the output of the fht
  int maxWhenY=0;
  int maxVal=0;
  for (int j = 1 ; j < FHT_N/2 ; j++)
  {
    if (fht_lin_out[j] > maxVal)
    {
      maxVal = fht_lin_out[j];
      maxWhenY = j;
    }
  }
  return maxWhenY;
}
// =================================================================================

// Calculate z-axis frequency function.
double zFFT()
{
  for (int k = 0 ; k < FHT_N ; k++) 
  { // save 256 samples
    if (accel.available())
    {
      accel.read();
      fht_input[k] = accel.z; // put real data into bins
      delay(1);
    }
  }
  fht_reorder(); // reorder the data before doing the fht
  fht_run(); // process the data in the fht
  fht_mag_lin(); // take the output of the fht
  int maxWhenZ=0;
  int maxVal=0;
  for (int k = 1 ; k < FHT_N/2 ; k++)
  {
    if (fht_lin_out[k] > maxVal)
    {
      maxVal = fht_lin_out[k];
      maxWhenZ = k;
    }
  }
  return maxWhenZ;
}
// =================================================================================
