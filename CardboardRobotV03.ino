
 /*
 //* Simple Motor Control for ESP32-WROOM + DRV8833
 //* Basic forward/reverse control without PWM speed control
 */
 //Added Bluetooth

//#include <Bluepad32.h>

// Motor control pins
const int motorPin1 = 19;  // GPIO1 - connects to IN1 on DRV8833
const int motorPin2 = 21;  // GPIO2 - connects to IN2 on DRV8833
const int motorPin3 = 22;  // GPIO1 - connects to IN1 on DRV8833
const int motorPin4 = 23;  // GPIO2 - connects to IN2 on DRV8833

void setup() {
  // Initialize pins as outputs
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  
  // Start with motor stopped
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);  

  Serial.begin(115200);
  Serial.println("Simple Motor Control Ready");
}

void loop() {
  // Motor forward for 2 seconds
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);  
  Serial.println("Forward");
  delay(2000);
  
  // Stop for 1 second
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  Serial.println("Stop");
  delay(1000);
  
  // Motor reverse for 2 seconds
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  Serial.println("Reverse");
  delay(2000);
  
  // Stop for 1 second
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  Serial.println("Stop");
  delay(1000);
}
