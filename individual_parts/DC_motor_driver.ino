#include <Arduino.h>
const int pwmPin = 25;        // GPIO25 for PWM
const int gpio17Pin = 17;     // GPIO17
const int gpio26Pin = 26;     // GPIO26
const int gpio16Pin = 16;     // GPIO16

const int pwmChannel = 0;     // LEDC PWM channel
const int pwmFrequency = 1000; // PWM frequency in Hz
const int pwmResolution = 8;  // 8-bit resolution (0-255)

// Slow start parameters
const int slowStartTime = 2000; // Total ramp-up time in milliseconds
const int maxDutyCycle = 128;   // 50% duty cycle (0-255 range)

// Function declaration for slowStart
void slowStart(int targetDutyCycle, int duration);

void setup() {
  // Set GPIO17, GPIO26, and GPIO16 as outputs
  pinMode(gpio17Pin, OUTPUT);
  pinMode(gpio26Pin, OUTPUT);
  pinMode(gpio16Pin, OUTPUT);

  // Pull GPIO26 and GPIO16 LOW to prevent a short
  digitalWrite(gpio26Pin, LOW);
  digitalWrite(gpio16Pin, LOW);

  // Pull GPIO17 HIGH
  digitalWrite(gpio17Pin, HIGH);

  // Configure PWM on GPIO25
  ledcSetup(pwmChannel, pwmFrequency, pwmResolution);
  ledcAttachPin(pwmPin, pwmChannel);

  // Initialize PWM with 0% duty cycle
  ledcWrite(pwmChannel, 0);

  // Perform slow start
  slowStart(maxDutyCycle, slowStartTime);
}

void loop() {
  // Nothing needed in the loop
}

// Function definition for slowStart
void slowStart(int targetDutyCycle, int duration) {
  int steps = 50; // Number of steps for the ramp-up
  int stepDelay = duration / steps; // Delay between each step
  int dutyIncrement = targetDutyCycle / steps; // Increment per step

  for (int duty = 0; duty <= targetDutyCycle; duty += dutyIncrement) {
    ledcWrite(pwmChannel, duty);
    delay(stepDelay);
  }

  // Ensure the final duty cycle is exactly the target value
  ledcWrite(pwmChannel, targetDutyCycle);
}
