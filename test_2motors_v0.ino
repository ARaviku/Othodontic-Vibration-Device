// This program takes the analog input from a potentiometer and
// maps it to a PWM value which is then used to
// change the power to two motors.

// Define input pins for H-bridge motor driver
#define PWM1 5 // pin 1
#define PWM2 6 // pin 9
#define left1 7 // pin 2
#define left0 8 // pin 7
#define right1 9 // pin 15
#define right0 10 // pin 10

// Initialize control and power variables
int control = 0;
int power = 0;

// ====================================================
void setup() {
  // Setup Arduino pins
  pinMode(A0,INPUT);
  pinMode(PWM1,OUTPUT);
  pinMode(PWM2,OUTPUT);
  pinMode(left1,OUTPUT);
  pinMode(left0,OUTPUT);
  pinMode(right1,OUTPUT);
  pinMode(right0,OUTPUT);

  // Setup directions of motor
  digitalWrite(left1,HIGH);
  digitalWrite(left0,LOW);
  digitalWrite(right1,HIGH);
  digitalWrite(right0,LOW);

  // Initialize serial monitor
  Serial.begin(9600);
  
}
// ====================================================
void loop() {
  // Read analog input from potentiometer
  control = analogRead(A0);
  
  // Map analog input to PWM input
  power = map(control,0,1023,0,255);
  
  // Apply PWM to enable pins
  analogWrite(PWM1,power);
  analogWrite(PWM2,power);

  // Display values on Serial Monitor/Plotter
  Serial.print(control);Serial.print("\t");
  Serial.print(power);Serial.print("\t");
  Serial.print(PWM1);Serial.print("\t");
  Serial.print(PWM2);Serial.println("\t");
  
  
}
