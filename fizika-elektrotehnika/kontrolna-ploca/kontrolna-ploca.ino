#define MAX_VOLTAGE 5.0 // V
#define MOSFET_THRESHOLD 2.0 // V

#define MAX_ANALOG_IN 6
#define MAX_DIGITAL_OUT 14
#define MAX_PAIRS 12

#define DELTA_TIME 1000

#define PRINT_SN_OUTPUT true
#define PRINT_HALL_VOLTAGE true
#define PRINT_FIRED_PAIRS true

// Coils:              1  2  3  4  5  6  7  8  9 10 11 12
int active_pairs[] = { 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 };
bool fired_pairs[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// In pins:     a0  a1  a2  a3  a4  a5
int a[] =      { 0,  0,  0,  0,  0,  0 };
int a_to_d[] = { 3,  5,  6,  9, 10, 11 };

// SN pins:        SN-1 SN-2 SN-3 
int sn[] =        {   2,   4,   8 };
bool sn_state[] = {   0,   0,   0 };

/* Launcher states:
    -1  > Fault
    0   > Ready
    1   > Launched
*/
int launcher_state = 0;
unsigned long previous_time;

float voltage = 0.0;

float sensor_to_voltage(int sensor_value);

void set_mosfet(int a_pin);

void setup() {
    
  // Serial monitor
  Serial.begin(9600);
  Serial.println("------------");

  // Set all mux outputs to A
  for (int i = 0; i < 3; ++i) {
    digitalWrite(sn[i], LOW);
  }

  // Record starting time
  previous_time = millis();

  pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {

  a[0] = analogRead(A0);
  a[1] = analogRead(A1);
  a[2] = analogRead(A2);
  a[3] = analogRead(A3);
  a[4] = analogRead(A4);
  a[5] = analogRead(A5);

  unsigned long current_time = millis();

  // Prints START

  if (current_time - previous_time >= DELTA_TIME) {

    previous_time = current_time;

    if (PRINT_SN_OUTPUT) {
      Serial.println("+---------+---------+---------+");
      Serial.print("|");
      for (int i = 0; i < 3; ++i) {
        Serial.print((String)" SN-" + (i + 1) + ": " + sn_state[i] + " |");
      }      
      Serial.println();
      Serial.println("+---------+---------+---------+");
    }

    if (PRINT_HALL_VOLTAGE) {
      Serial.println("+--------------+--------------+");
      Serial.print("|");
      for (int i = 0; i < MAX_ANALOG_IN; ++i) {
        Serial.print((String)"  A" + i + ": " + sensor_to_voltage(a[i]) + " V  |");
        if (i % 2 && i < MAX_ANALOG_IN - 1) {
          Serial.println();
          Serial.print("|");
        }
      }
      Serial.println();
      Serial.println("+--------------+--------------+");
    }

    if (PRINT_FIRED_PAIRS) {
      Serial.println("+---+---+---+---+---+---+---+---+---+---+---+---+");
      Serial.print("|");
      for (int i = 0; i < MAX_PAIRS; ++i) {
        if (fired_pairs[i]) {
          Serial.print(" # |");
        } else {
          Serial.print("   |");
        }  
      }
      Serial.println("");
      Serial.println("+---+---+---+---+---+---+---+---+---+---+---+---+");
    }

  }

  // Prints END

  bool all_active_fired = true;
  for (int a = 0; a < (MAX_PAIRS / 2); ++a) {
    if (active_pairs[a] || active_pairs[a + (MAX_PAIRS / 2)]) {
      set_mosfet(a);
    }
    if (active_pairs[a] != fired_pairs[a] || active_pairs[a + (MAX_PAIRS / 2)] != fired_pairs[a + (MAX_PAIRS / 2)]) {
      all_active_fired = false;
    }
  }
  if (all_active_fired) {
    launcher_state = 1;    
  }

}

float sensor_to_voltage(int sensor_value) {
  return sensor_value * (MAX_VOLTAGE / 1023.0);
}

void set_mosfet(int a_pin) {
  if (sensor_to_voltage(a[a_pin]) <= MOSFET_THRESHOLD) {

    // Activate MOSFET
    analogWrite(a_to_d[a_pin], 255);

    // Mark that this pair was turned on
    fired_pairs[a_pin + sn_state[a_pin % 6 / 2] * (MAX_PAIRS / 2)] = 1;

    // If this was the last Hall effect pair for this SN
    if (a_pin % 2 && a_pin < MAX_ANALOG_IN / 2) {
      // Switch (de)mux in/out (from A to B)
      digitalWrite(sn[a_pin / 2], HIGH);
      sn_state[a_pin / 2] = HIGH;
    }

  } else {
    
    // Deactivate MOSFET
    analogWrite(a_to_d[a_pin], 0);

  }
}
/*
digitalWrite(LED_BUILTIN, HIGH);
    voltage = 4.8;
    delay(10000);
*/