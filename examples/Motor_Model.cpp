#include <Arduino.h>

#include "MotorController.h"

// This pin is attached to PWM input of
// motor driver to control speed
#define PWM_PIN D1
// This pin is attached to direction control
// input of motor driver
#define DIRECTION_PIN D3
// Encoder outputs attached to these
// interrupt pins
#define ENCODER_PIN_A D5
#define ENCODER_PIN_B D6

// Frequency at which control loop will be run
#define CONTROL_ROUTINE_CALL_FREQUENCY 50 // Hz

// Number of quadrature counts to complete
// one full rotation of the shaft
#define ENCODER_COUNTS_PER_ROTATION 4160

// Time interval after which things are
// printed to serial monitor
#define SERIAL_PRINT_TIME_PERIOD 1E6 // us

// Motor controller object to 
// control motor angular velocity
MotorController motor_controller(
  DIRECTION_PIN,
  ENCODER_PIN_A,
  ENCODER_PIN_B,
  CONTROL_ROUTINE_CALL_FREQUENCY,
  ENCODER_COUNTS_PER_ROTATION
);

// Variable to fetch and store the current time
float current_time;

// Variable to store when the last angular state
// update was performed in microseconds
float last_control_routine_call_time;

// Variable to store when the last serial print
// was performed in microsecond
float last_serial_print_time;

void setup()
{
	// Initialize the Serial Monitor
	Serial.begin(115200);

	// The following functions are not available
	// in Arduino Uno or Mega chips, comment them
	// PWM Frequency input to motor driver
	// Should be between 5 ~ 15 kHz
	analogWriteFreq(10E3);
	// Bit-Resolution for representing PWM Duty cycle
	analogWriteResolution(8);
	// Set max controller output according to bit resolution of PWM Duty cycle
	motor_controller.setMaxControllerOutput((1 << 8) - 1);

	// Initialize motor controller
	motor_controller.setPIDGains(3500, 1250, 2);

	motor_controller.enablePIDControl();

	// Initialize global variables	
	current_time = micros();
	last_control_routine_call_time	= current_time;
	last_serial_print_time			= current_time;

	// Set motor controller target to 10 rad/s
	motor_controller.setTargetStateValue(10);

	Serial.println("Initialized successfully");
}

void loop()
{
	current_time = micros();

	if (current_time - last_control_routine_call_time > 1E6/CONTROL_ROUTINE_CALL_FREQUENCY)
	{
		motor_controller.updateAngularVelocity();
		motor_controller.spinMotor();

		analogWrite(PWM_PIN, motor_controller.getControllerOutput());

		last_control_routine_call_time = current_time;
	}

	if (current_time - last_serial_print_time > SERIAL_PRINT_TIME_PERIOD)
	{
		Serial.print("Velocity:\t");
		Serial.println(motor_controller.getMotorAngularVelocity());

		Serial.print("Error: ");
		Serial.println(motor_controller.getError());

		Serial.print("PID Output: ");
		Serial.println(motor_controller.getControllerOutput());

		last_serial_print_time = current_time;
	}
}