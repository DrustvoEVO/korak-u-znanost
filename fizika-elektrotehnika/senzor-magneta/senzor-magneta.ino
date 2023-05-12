#define MAX_VOLTAGE 5.0

void setup() {
  // Serial Monitor setup (for printing)
  Serial.begin(9600);
  Serial.println("------------");

  // LED setup
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // Read form analog input
  int sensorValue = analogRead(A0);

  // Calculate the voltage
  float voltage = sensorValue * (MAX_VOLTAGE / 1023.0);
  Serial.println((String)"V_out: " + voltage);
  
  // LED controls
  if (voltage > 0.8) {
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}
