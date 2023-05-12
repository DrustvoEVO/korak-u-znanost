#define MAX_VOLTAGE 5.0

void setup() {
  // Serial Monitor setup (for printing)
  Serial.begin(9600);
  Serial.println("------------");

  // LED setup
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // Read form analog inputs
  int sensorValue1 = analogRead(A0);
  int sensorValue2 = analogRead(A1);

  // Calculate the voltage
  float voltage1 = sensorValue1 * (MAX_VOLTAGE / 1023.0);
  float voltage2 = sensorValue2 * (MAX_VOLTAGE / 1023.0);
  
  /*
    Printing functions are often very slow,
    so we don't print to have perfect timing
    */

  // Serial.println((String)"V_out: " + voltage);
  
  // LED controls
  if (voltage1 > 0.8 && voltage2 > 0.8) {
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }

  // MOSFET contorls, separated

  if (voltage1 > 0.8) {
    analogWrite(3, 0);
  } else {
    analogWrite(3, 255);
  }

  if (voltage2 > 0.8) {
    analogWrite(5, 0);
  } else {
    analogWrite(5, 255);
  }
}
