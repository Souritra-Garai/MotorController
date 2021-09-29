/**
 * @file SpinMotorExample.cpp
 * @author Navneet Kaur (navneet.kaur@iitgn.ac.in)
 * @brief Example for running PID controls for a target velocity.
 * The angular velocity is updated at a rate of 50Hz.  PID output
 * and angular velocity is printed at every 2000 ms rate.
 * @version 0.1
 * @date 2021-06-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "MotorController.h"

#define PWM_PIN D1
#define DIRECTION_PIN D3
#define ENCODER_PIN_A D5
#define ENCODER_PIN_B D6


#define VELOCITY_UPDATE_FREQUENCY 50 // Hz

#define ENCODER_COUNTS_PER_ROTATION 4160

#define PRINT_TIME_PERIOD 1000 // ms

#define MAX_PWM ((1 << 15) - 1)

MotorController motor_controller(
  DIRECTION_PIN,
  ENCODER_PIN_A,
  ENCODER_PIN_B,
  VELOCITY_UPDATE_FREQUENCY,
  ENCODER_COUNTS_PER_ROTATION
);

long int last_print_time;
long int last_vel_update_time;

void setup()
{
  Serial.begin(115200);

  // analogWriteFreq(10000); 
  analogWriteResolution(15);

  last_vel_update_time = micros();
  last_print_time = millis();

  motor_controller.setPIDGains(3000, 2500, 0);

  motor_controller.enablePIDControl();

  motor_controller.setTargetStateValue(10);

  Serial.println("Initialized successfully");
}

void loop()
{
  if (millis() - last_print_time > PRINT_TIME_PERIOD)
  {
    Serial.print("Velocity:\t");
    Serial.println(motor_controller.getMotorAngularVelocity());

    Serial.print("Error: ");
    Serial.println(motor_controller.getError());

    Serial.print("PID Output: ");
    Serial.println(motor_controller.getPIDControlOutput());

    Serial.print("Encoder reading: ");
    Serial.println(motor_controller.read());

    last_print_time = millis();
  }
  
  if (micros() - last_vel_update_time > 1E6/VELOCITY_UPDATE_FREQUENCY)
  {
    motor_controller.updateAngularVelocity();
    motor_controller.spinMotor();

    analogWrite(PWM_PIN, motor_controller.getPIDControlOutput());

    last_vel_update_time = micros();
  }
}