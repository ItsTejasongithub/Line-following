//**********Line Following Robot using Arduino UNO**********//
//**********Using 5 Channel IR Sensor and L298P Motor Driver**********************// 

#include <Arduino.h>

// L298P Motor Driver Pin Configuration
#define pwmA 10   // Speed control for Motor A
#define pwmB 11   // Speed control for Motor B
#define enA 12    // Direction control for Motor A
#define enB 13    // Direction control for Motor B

//**********5 Channel IR Sensor Connection**********//
#define ir1 A0    // Left Most IR Sensor
#define ir2 A1    // Left IR Sensor
#define ir3 A2    // Middle IR Sensor
#define ir4 A3    // Right IR Sensor
#define ir5 A4    // Right Most IR Sensor
//*************************************************//

void setup() {
  pinMode(pwmA, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  pinMode(ir3, INPUT);
  pinMode(ir4, INPUT);
  pinMode(ir5, INPUT);
}

// Function to control motor movement
void moveMotors(int rightSpeed, int leftSpeed, bool rightForward, bool leftForward) {
  // Enable both motors
  digitalWrite(enA, HIGH);
  digitalWrite(enB, HIGH);
  
  // Set motor speeds and directions (0-255)
  if (rightForward) {
    analogWrite(pwmA, rightSpeed);
    digitalWrite(enA, HIGH);
  } else {
    analogWrite(pwmA, 0);
    digitalWrite(enA, LOW);
  }
  
  if (leftForward) {
    analogWrite(pwmB, leftSpeed);
    digitalWrite(enB, HIGH);
  } else {
    analogWrite(pwmB, 0);
    digitalWrite(enB, LOW);
  }
}

void loop() {
  // Reading Sensor Values
  int s1 = digitalRead(ir1);  
  int s2 = digitalRead(ir2);  
  int s3 = digitalRead(ir3);
  int s4 = digitalRead(ir4);  
  int s5 = digitalRead(ir5);  

  // Motor speed - you can adjust this value between 0-255
  int motorSpeed = 60;

  // If only middle sensor detects black line
  if((s1 == 1) && (s2 == 1) && (s3 == 0) && (s4 == 1) && (s5 == 1))
  {
    // Going forward with full speed 
    moveMotors(motorSpeed, motorSpeed, true, true);
  }
  
  // If only left sensor detects black line
  if((s1 == 1) && (s2 == 0) && (s3 == 1) && (s4 == 1) && (s5 == 1))
  {
    // Going right with full speed 
    moveMotors(motorSpeed, 0, true, false);
  }
  
  // If only left most sensor detects black line
  if((s1 == 0) && (s2 == 1) && (s3 == 1) && (s4 == 1) && (s5 == 1))
  {
    // Going right with full speed 
    moveMotors(motorSpeed, motorSpeed, true, false);
  }

  // If only right sensor detects black line
  if((s1 == 1) && (s2 == 1) && (s3 == 1) && (s4 == 0) && (s5 == 1))
  {
    // Going left with full speed 
    moveMotors(0, motorSpeed, false, true);
  }

  // If only right most sensor detects black line
  if((s1 == 1) && (s2 == 1) && (s3 == 1) && (s4 == 1) && (s5 == 0))
  {
    // Going left with full speed 
    moveMotors(motorSpeed, motorSpeed, false, true);
  }

  // If middle and right sensor detects black line
  if((s1 == 1) && (s2 == 1) && (s3 == 0) && (s4 == 0) && (s5 == 1))
  {
    // Going left with full speed 
    moveMotors(0, motorSpeed, false, true);
  }

  // If middle and left sensor detects black line
  if((s1 == 1) && (s2 == 0) && (s3 == 0) && (s4 == 1) && (s5 == 1))
  {
    // Going right with full speed 
    moveMotors(motorSpeed, 0, true, false);
  }

  // If middle, left and left most sensor detects black line
  if((s1 == 0) && (s2 == 0) && (s3 == 0) && (s4 == 1) && (s5 == 1))
  {
    // Going right with full speed 
    moveMotors(motorSpeed, 0, true, false);
  }

  // If middle, right and right most sensor detects black line
  if((s1 == 1) && (s2 == 1) && (s3 == 0) && (s4 == 0) && (s5 == 0))
  {
    // Going left with full speed 
    moveMotors(0, motorSpeed, false, true);
  }

  // If all sensors are on a black line
  if((s1 == 0) && (s2 == 0) && (s3 == 0) && (s4 == 0) && (s5 == 0))
  {
    // Stop
    moveMotors(0, 0, false, false);
  }
}
