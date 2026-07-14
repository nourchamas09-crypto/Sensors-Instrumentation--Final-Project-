/*
  Servo Motor Test
  Autonomous Firefighting and Rescue Robot

  Purpose:
  This code tests the MG996R servo motor separately before integrating it
  into the full robot code.
*/

#include <ESP32Servo.h>

#define SERVO_PIN 23

Servo nozzleServo;

void setup() {
  Serial.begin(115200);

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  nozzleServo.setPeriodHertz(50);
  nozzleServo.attach(SERVO_PIN, 500, 2400);

  Serial.println("Servo test started");
}

void loop() {
  Serial.println("Moving to 0 degrees");
  nozzleServo.write(0);
  delay(1500);

  Serial.println("Moving to 90 degrees");
  nozzleServo.write(90);
  delay(1500);

  Serial.println("Moving to 180 degrees");
  nozzleServo.write(180);
  delay(1500);
}
