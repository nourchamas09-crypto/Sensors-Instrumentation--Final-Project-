/*
  HC-SR04 Ultrasonic Sensor Test
  Autonomous Firefighting and Rescue Robot

  Purpose:
  This code tests the ultrasonic sensor separately before integrating it
  into the full robot code.

  Important:
  The HC-SR04 ECHO pin outputs 5V, so it should be connected to the ESP32
  through a voltage divider.
*/

#define TRIG_PIN 5
#define ECHO_PIN 18

long duration;
float distance_cm;

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.println("HC-SR04 Ultrasonic Sensor Test");
  Serial.println("time_ms,distance_cm");
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) {
    Serial.print(millis());
    Serial.println(",No echo");
  } else {
    distance_cm = duration * 0.0343 / 2.0;

    Serial.print(millis());
    Serial.print(",");
    Serial.println(distance_cm);
  }

  delay(500);
}
