# Self-Balancing Bot

A beginner-level two-wheel self-balancing robot built using an Arduino Uno, MPU6050 sensor, A4988 stepper motor drivers, and NEMA17 stepper motors.

The bot works on the principle of an inverted pendulum. It continuously measures its tilt angle using the MPU6050 and adjusts the speed and direction of the motors using a PID controller so that it remains upright.

---

# Components Used

* Arduino Uno
* MPU6050 Gyroscope + Accelerometer
* 2 × NEMA17 Stepper Motors
* 2 × A4988 Stepper Motor Drivers
* HC-05 Bluetooth Module
* 12 V Battery
* Chassis and Wheels
* 100 µF Capacitor across each A4988 VMOT supply
* Jumper Wires and Breadboard

---

# Working Principle

The robot constantly performs the following steps:

1. Read the tilt angle from the MPU6050.
2. Compare the measured angle with the desired angle (`setPoint`).
3. Calculate the error.
4. Use a PID controller to determine how fast the motors should rotate.
5. Move the wheels in the same direction as the tilt to bring the robot back to the balanced position.

The bot also supports Bluetooth control:

* `F` → Move Forward
* `B` → Move Backward
* `S` → Stay Balanced / Stop

---

# Circuit Connections

## MPU6050

| MPU6050 Pin | Arduino Uno |
| ----------- | ----------- |
| VCC         | 5V          |
| GND         | GND         |
| SDA         | A4          |
| SCL         | A5          |

## Bluetooth Module (HC-05)

| HC-05 Pin | Arduino Uno |
| --------- | ----------- |
| VCC       | 5V          |
| GND       | GND         |
| TX        | Pin 10      |
| RX        | Pin 11      |

## Left Motor Driver (A4988)

| A4988 Pin | Arduino Uno |
| --------- | ----------- |
| STEP      | Pin 5       |
| DIR       | Pin 4       |
| EN        | GND / LOW   |

## Right Motor Driver (A4988)

| A4988 Pin | Arduino Uno |
| --------- | ----------- |
| STEP      | Pin 3       |
| DIR       | Pin 2       |
| EN        | GND / LOW   |

Notes:

* Connect MS1, MS2 and MS3 to GND for full-step mode.
* Connect a 100 µF capacitor between VMOT and GND near each A4988 driver.
* Common ground must be shared between Arduino, battery and drivers.

---

# PID Parameters

Current stable values used:

```cpp
float Kp = 28;
float Ki = 0;
float Kd = 1.0;
```

Explanation:

* `Kp` controls how strongly the bot reacts to tilt.
* `Ki` corrects long-term drift.
* `Kd` reduces oscillation.

---

# Sample Control Logic

```cpp
error = setPoint - input;
output = (Kp * error) + (Ki * cumError) + (Kd * rateError);
```

The calculated `output` is then used to control the motor speed.

---

# Features

* Self-balancing using PID control
* Forward and backward motion through Bluetooth
* Stable hold position
* Adjustable PID values
* Protection against large tilt angles

---

# Common Problems and Solutions

## Motors vibrate but do not rotate

* Motor coils connected incorrectly
* Current limit on A4988 too low or too high
* Stepper speed too high
* Missing common ground

## Driver becomes very hot

* Current limit is too high
* Add a heatsink and reduce Vref
* Recommended Vref: around 0.5–0.7 V depending on motor and driver

## MPU6050 values do not change

* Check SDA and SCL connections
* SDA → A4
* SCL → A5

---

# Applications

* Self-balancing transport systems
* Delivery robots
* Humanoid robot balancing
* Educational robotics projects
* Control systems and PID learning

---

