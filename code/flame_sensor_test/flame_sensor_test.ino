/*
  Flame Sensor Test
  Autonomous Firefighting and Rescue Robot

  Purpose:
  This code tests the flame sensor separately before integrating it
  into the full robot code.

  The flame sensor analog output is connected to GPIO34.
  The ESP32 reads the analog value and prints it to the Serial Monitor.
*/

#define FLAME_PIN 34

int flameValue = 0;

// Fire is detected when flameValue is lower than this threshold.
// This value was adjusted during testing to avoid false detection.
int flameThreshold = 1000;

void setup() {
  Serial.begin(115200);

  pinMode(FLAME_PIN, INPUT);

  Serial.println("Flame Sensor Test");
  Serial.println("time_ms,flame_raw,fire_detected");
}

void loop() {
  flameValue = analogRead(FLAME_PIN);

  bool fireDetected = flameValue < flameThreshold;

  Serial.print(millis());
  Serial.print(",");
  Serial.print(flameValue);
  Serial.print(",");
  Serial.println(fireDetected);

  delay(500);
}
