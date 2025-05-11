#include <Arduino.h>
// IR 5 Sensor Array Line Following Code
// For TCRT5000L Infrared sensors
// White > 800, Black < 400
// Updated for L298N Motor Driver

// Define the analog pins for the five sensors
const int sensorPins[5] = {A0, A1, A2, A3, A4};  // Modify these pins as per your connections

// Threshold for black/white detection
const int BLACK_THRESHOLD = 400;  // Values below this are considered black
const int WHITE_THRESHOLD = 800;  // Values above this are considered white

// Variables to store sensor readings
int sensorValues[5];
bool lineDetected[5];  // true if black line is detected

// Motor control pins for L298N Motor Driver
const int enA = 5;     // PWM pin for right motor speed control
const int in1 = 6;     // Right motor direction control 1
const int in2 = 7;     // Right motor direction control 2
const int enB = 10;    // PWM pin for left motor speed control
const int in3 = 8;     // Left motor direction control 1
const int in4 = 9;     // Left motor direction control 2

// Function prototypes
void readSensors();
void printSensorValues();
int getLinePosition();
void followLine(int position);
void setMotorSpeeds(int leftSpeed, int rightSpeed);
void searchForLine();

// Speed settings
const int baseSpeed = 50;  // Base speed (0-255)
const int turnSpeed = 50;  // Speed during turns (0-255)

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  
  // Initialize sensor pins
  for (int i = 0; i < 5; i++) {
    pinMode(sensorPins[i], INPUT);
  }
  
  // Initialize motor control pins for L298N
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  // Make sure motors are stopped at startup
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  
  // Initial delay to give time to position the robot
  delay(2000);
  
  Serial.println("IR Sensor Array Line Following with L298N");
  Serial.println("=========================================");
}

void loop() {
  // Read all sensors
  readSensors();
  
  // Display sensor values for debugging
  printSensorValues();
  
  // Determine the line position
  int linePosition = getLinePosition();
  
  // Display line position for debugging
  Serial.print("Line position: ");
  switch (linePosition) {
    case -2: Serial.println("FAR LEFT"); break;
    case -1: Serial.println("LEFT"); break;
    case 0: Serial.println("CENTER"); break;
    case 1: Serial.println("RIGHT"); break;
    case 2: Serial.println("FAR RIGHT"); break;
    case 999: Serial.println("NO LINE"); break;
    default: Serial.println("UNKNOWN"); break;
  }
  
  // Control motors based on line position
  followLine(linePosition);
  
  delay(50);  // Short delay for stability
}

void readSensors() {
  for (int i = 0; i < 5; i++) {
    sensorValues[i] = analogRead(sensorPins[i]);
    lineDetected[i] = (sensorValues[i] < BLACK_THRESHOLD);
  }
}

void printSensorValues() {
  Serial.print("Sensor Values: ");
  for (int i = 0; i < 5; i++) {
    Serial.print(sensorValues[i]);
    Serial.print("\t");
  }
  Serial.print("Line Detected: ");
  for (int i = 0; i < 5; i++) {
    Serial.print(lineDetected[i]);
    Serial.print("\t");
  }
  Serial.println();
}

int getLinePosition() {
  // Calculate weighted average for line position
  // Return values: -2 (far left), -1 (left), 0 (center), 1 (right), 2 (far right), 999 (no line)
  
  if (!lineDetected[0] && !lineDetected[1] && !lineDetected[2] && !lineDetected[3] && !lineDetected[4]) {
    return 999;  // No line detected
  }
  
  // Weighted average calculation
  int sum = 0;
  int weightedSum = 0;
  
  for (int i = 0; i < 5; i++) {
    if (lineDetected[i]) {
      sum += 1;
      weightedSum += (i - 2) * 10;  // -20, -10, 0, 10, 20 as position values
    }
  }
  
  if (sum == 0) return 999;
  
  int position = weightedSum / sum;
  
  // Convert to discrete positions for simplicity
  if (position < -15) return -2;       // Far left
  else if (position < -5) return -1;   // Left
  else if (position < 5) return 0;     // Center
  else if (position < 15) return 1;    // Right
  else return 2;                       // Far right
}

void followLine(int position) {
  // Control motors based on line position
  switch (position) {
    case -2:  // Far left - Sharp left turn
      setMotorSpeeds(0, turnSpeed);
      break;
      
    case -1:  // Left - Gentle left turn
      setMotorSpeeds(baseSpeed/2, baseSpeed);
      break;
      
    case 0:   // Center - Go straight
      setMotorSpeeds(baseSpeed, baseSpeed);
      break;
      
    case 1:   // Right - Gentle right turn
      setMotorSpeeds(baseSpeed, baseSpeed/2);
      break;
      
    case 2:   // Far right - Sharp right turn
      setMotorSpeeds(turnSpeed, 0);
      break;
      
    case 999: // No line - Stop or implement search pattern
      // Option 1: Stop
      setMotorSpeeds(0, 0);
      
      // Option 2: Implement a search pattern (uncomment to use)
      // searchForLine();
      break;
  }
}

void setMotorSpeeds(int leftSpeed, int rightSpeed) {
  // Set left motor speed and direction (using in3, in4, enB)
  if (leftSpeed >= 0) {
    // Forward direction for left motor
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enB, leftSpeed);
  } else {
    // Backward direction for left motor
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enB, -leftSpeed);
  }
  
  // Set right motor speed and direction (using in1, in2, enA)
  if (rightSpeed >= 0) {
    // Forward direction for right motor
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enA, rightSpeed);
  } else {
    // Backward direction for right motor
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(enA, -rightSpeed);
  }
}

// Optional function to implement a search pattern when line is lost
void searchForLine() {
  // Simple example: rotate in place to find the line again
  setMotorSpeeds(-baseSpeed, baseSpeed);
  
  // More advanced implementation could include:
  // - Remembering last known line position
  // - Systematic search patterns
  // - Timeout mechanism to stop if line not found
}
