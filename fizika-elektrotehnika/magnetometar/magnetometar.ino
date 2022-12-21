#define MAX_VOLTAGE 5.0
#define HALL_OUT_MIN 0.86
#define HALL_OUT_MAX 4.21
#define SENSITIVITY 0.0025

float gauss_max;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("------------");
  Serial.println((String)"HALL_OUT_MIN: " + HALL_OUT_MIN);
  Serial.println((String)"HALL_OUT_MAX: " + HALL_OUT_MAX);
  Serial.println((String)"SENSITIVITY: " + SENSITIVITY);
  gauss_max = 0.0;
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (MAX_VOLTAGE / 1023.0);
  Serial.println((String)"V: " + voltage);
  // float gauss = ( voltage - HALL_OUT_MIN - (HALL_OUT_MAX - HALL_OUT_MIN) / 2 ) / SENSITIVITY - 130.0;
  float gauss = (voltage - 2.51) / SENSITIVITY;
  if (abs(gauss) > abs(gauss_max)) {
      gauss_max = gauss;
  }
  Serial.println((String)"G: " + gauss);
  Serial.println((String)"G_max: " + gauss_max);
  Serial.println((String)"----------");
  delay(500);
}
