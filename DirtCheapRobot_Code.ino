#include <Bluepad32.h>
// Dirt Cheap Robot
// Paul McCabe

// === Motor driver pins ===
#define IN1 19   // Motor A input 1
#define IN2 21   // Motor A input 2
#define IN3 22   // Motor B input 1
#define IN4 23   // Motor B input 2

// === PWM setup ===
#define PWM_FREQ 1000
#define PWM_RES 8
#define CH_IN1 0
#define CH_IN2 1
#define CH_IN3 2
#define CH_IN4 3

GamepadPtr myGamepad = nullptr;

// === Function prototypes ===
void onConnectedGamepad(GamepadPtr gp);
void onDisconnectedGamepad(GamepadPtr gp);
void handleArcadeDrive();

// === Setup ===
void setup() {
  Serial.begin(115200);
  Serial.println("Starting Bluepad32 - Arcade Drive Mode");

  // Motor pin setup
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Attach and configure PWM
  ledcAttachPin(IN1, CH_IN1);
  ledcAttachPin(IN2, CH_IN2);
  ledcAttachPin(IN3, CH_IN3);
  ledcAttachPin(IN4, CH_IN4);

  ledcSetup(CH_IN1, PWM_FREQ, PWM_RES);
  ledcSetup(CH_IN2, PWM_FREQ, PWM_RES);
  ledcSetup(CH_IN3, PWM_FREQ, PWM_RES);
  ledcSetup(CH_IN4, PWM_FREQ, PWM_RES);

  // Initialize Bluepad32
  BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);
  BP32.forgetBluetoothKeys();  // optional

  Serial.println("Waiting for controller...");
}

// === Loop ===
void loop() {
  BP32.update();

  if (myGamepad && myGamepad->isConnected()) {
    handleArcadeDrive();
  } else {
    // stop all motors
    ledcWrite(CH_IN1, 0);
    ledcWrite(CH_IN2, 0);
    ledcWrite(CH_IN3, 0);
    ledcWrite(CH_IN4, 0);
  }

  delay(10);
}

// === Bluepad32 callbacks ===
void onConnectedGamepad(GamepadPtr gp) {
  myGamepad = gp;
  Serial.println("✅ Controller connected!");
}

void onDisconnectedGamepad(GamepadPtr gp) {
  if (myGamepad == gp) {
    myGamepad = nullptr;
    Serial.println("❌ Controller disconnected!");
  }
}

// === Utility: map joystick to PWM ===
int mapJoystickToPWM(int value) {
  int pwm = map(abs(value), 0, 512, 0, 255);
  return constrain(pwm, 0, 255);
}

// === Drive helper ===
void driveMotor(int channelA1, int channelA2, int speed) {
  if (speed > 0) {
    ledcWrite(channelA1, speed);
    ledcWrite(channelA2, 0);
  } else if (speed < 0) {
    ledcWrite(channelA1, 0);
    ledcWrite(channelA2, abs(speed));
  } else {
    ledcWrite(channelA1, 0);
    ledcWrite(channelA2, 0);
  }
}

// === Core control logic ===
void handleArcadeDrive() {
  int forward = -myGamepad->axisY();  // forward/back (invert if needed)
  int turn    = myGamepad->axisRX();  // right stick X axis

  // --- Deadzone ---
  if (abs(forward) < 30) forward = 0;
  if (abs(turn) < 30) turn = 0;

  // --- Mix for arcade drive ---
  int leftSpeed  = forward + turn;
  int rightSpeed = forward - turn;

  // Clamp to joystick range (-512 to 512)
  leftSpeed  = constrain(leftSpeed, -512, 512);
  rightSpeed = constrain(rightSpeed, -512, 512);

  // Map to PWM (0–255)
  int leftPWM  = mapJoystickToPWM(leftSpeed);
  int rightPWM = mapJoystickToPWM(rightSpeed);

  // Apply sign (direction)
  driveMotor(CH_IN1, CH_IN2, leftSpeed >= 0 ? leftPWM : -leftPWM);
  driveMotor(CH_IN3, CH_IN4, rightSpeed >= 0 ? rightPWM : -rightPWM);
}
