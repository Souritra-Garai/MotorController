/**
 * @file VelocityCalculationTimer.cpp
 * @author Souritra Garai (souritra.garai@iitgn.ac.in)
 * @brief This is an example code to estimate angular velocity 
 * of a rotating shaft with an encoder attached to it,
 * using the AngularVelocityCalculator class.
 * 
 * The angular velocity is updated at frequency of 50 Hz,
 * using Timer 1 interrupt. It is printed to serial monitor
 * at a much slower rate of 1Hz.
 * 
 * @version 0.2
 * @date 2021-06-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "AngularVelocityCalculator.h"

#include <Arduino.h>

#define PWM_PIN D2

// Encoder outputs attached to these
// interrupt pins on the Arduino Mega board
#define ENCODER_PIN_A D5
#define ENCODER_PIN_B D6

/**
 * @brief Frequency at which velocity will be updated
 */
#define VELOCITY_UPDATE_FREQUENCY 50 // Hz

/**
 * @brief Number of quadrature counts to complete
 * one full rotation of the shaft
 */
#define ENCODER_COUNTS_PER_ROTATION 4160

/**
 * @brief Our object to estimate the angular velocity of encoder shaft
 */
AngularVelocityCalculator encoder_shaft(
  ENCODER_PIN_A,
  ENCODER_PIN_B,
  VELOCITY_UPDATE_FREQUENCY,
  ENCODER_COUNTS_PER_ROTATION
);

// To verify the time period of velocity update 

/**
 * @brief Variable to store the sum of durations in 
 * microseconds after which velocity is updated
 */
float velocity_sum;
float velocity_sqr_sum;
float num_obs;

float velocity_mean;
float velocity_std_dev;

unsigned int pwm;
float velocity;
/**
 * @brief Variable to store when the last velocity update 
 * was performed in microsecond
 */
float last_velocity_update_time;

void setup()
{
  /**
   * @brief Initialize Serial Comm
   */
  Serial.begin(115200);  

  analogWriteFreq(10000);
  analogWriteResolution(15);
  
  Serial.println("NodeMCU Initialized successfully");

  velocity_sum = 0;
  velocity_sqr_sum = 0;
  num_obs = 0;

  velocity_mean = 0;
  velocity_std_dev = 1000;
}

void loop()
{

  float current_time = millis();
  // Angular velocity update loop
  if (micros() - last_velocity_update_time > 1E6 / VELOCITY_UPDATE_FREQUENCY)
  {
    encoder_shaft.updateAngularVelocity();

    // if (num_obs > 5)
    // {
    //   float angular_velocity;
    //   encoder_shaft.getAngularVelocity(angular_velocity);

    //   velocity_sum += angular_velocity;
    //   velocity_sqr_sum += angular_velocity * angular_velocity;

    //   velocity_mean = velocity_sum / (num_obs - 5);
    //   velocity_std_dev = sqrt(velocity_sqr_sum / (num_obs - 5) - velocity_sum * velocity_sum);

    //   Serial.println(angular_velocity);
    // }

    // num_obs += 1;

    encoder_shaft.getAngularVelocity(velocity);


    last_velocity_update_time = micros();
  }

  // Sampling
  if ()
  {
    pwm = sin(current_time * omega) * 1<<15;

    analogWrite

    Serial.print(current_time)
    Serial.print(',')
    Serial.print(pwm)
    ,
    velocity
  }



  if (velocity_std_dev < 0.1 && pwm <= 32767U)
  {
    // Serial.print(pwm);
    // Serial.print(',');
    // Serial.println(velocity_mean);

    pwm += 1;
    analogWrite(PWM_PIN, pwm);

    velocity_sum = 0;
    velocity_sqr_sum = 0;
    num_obs = 0;

    velocity_mean = 0;
    velocity_std_dev = 1000;
  }
}
