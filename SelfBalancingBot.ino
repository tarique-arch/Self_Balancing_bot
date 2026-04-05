#include <Wire.h>
#include <MPU6050_tockn.h>
#include <AccelStepper.h>
#include <SoftwareSerial.h>

// -------- Motor Pins (A4988) --------
#define LEFT_STEP 5
#define LEFT_DIR  4
#define RIGHT_STEP 3
#define RIGHT_DIR 2

// -------- Bluetooth --------
SoftwareSerial BT(10, 11); // RX, TX

// -------- MPU6050 --------
MPU6050 mpu6050(Wire);

// -------- Stepper Motors --------
AccelStepper leftStepper(1, LEFT_STEP, LEFT_DIR);
AccelStepper rightStepper(1, RIGHT_STEP, RIGHT_DIR);

// -------- PID Constants (Safe starting values) --------
float Kp = 28;
float Ki = 0;
float Kd = 1;

// -------- PID Variables --------
float setPoint = 0;
float input;
float output;

float error;
float lastError = 0;
float cumError = 0;
float rateError;

void setup() {

  Serial.begin(9600);
  BT.begin(9600);

  // MPU6050 setup
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  // Stepper setup
  leftStepper.setMaxSpeed(1500);
  rightStepper.setMaxSpeed(1500);

  Serial.println("Balancing bot ready");
}

void loop() {

  // -------- Update MPU --------
  mpu6050.update();

  // Change axis if needed depending on MPU orientation
  input = mpu6050.getAngleY();

  // -------- Bluetooth Commands --------
  if (BT.available()) {

    char command = BT.read();

    if (command == 'F') {
      setPoint = -2;    // move forward
    }

    else if (command == 'B') {
      setPoint = 2;     // move backward
    }

    else if (command == 'S') {
      setPoint = 0;     // hold balance
    }
  }

  // -------- PID Calculation --------
  error = setPoint - input;

  cumError += error;
  rateError = error - lastError;

  output = (Kp * error) + (Ki * cumError) + (Kd * rateError);

  lastError = error;

  if(abs(error) < 0.3){  // added later(dead zone around zero)
    output = 0;
    // cumError = 0;
}
  // -------- Limit motor speed --------
  output = constrain(output, -600, 600);

  if (abs(input) > 8 && abs(input)<45) {
    output *= 1.2;
  }
  if (abs(input) > 15 && abs(input)<45) {
    output *= 1.5;
  }
  if (abs(input) > 25 && abs(input)<45) {
    output *= 2.5;
  }
  
  // -------- Safety if bot falls --------
  if (abs(input) > 45) {
    leftStepper.setSpeed(0);
    rightStepper.setSpeed(0);

  } else {
    leftStepper.setSpeed(output);
    rightStepper.setSpeed(-output);
  }

  leftStepper.runSpeed();
  rightStepper.runSpeed();

  // -------- Debug (optional) --------
   Serial.println(input);
  //  Serial.println(output);


  // -------- Stable loop timing --------
  delay(5);
}
