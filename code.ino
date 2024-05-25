// Define pins for relay control
#define RELAY1_PIN 6 // Left front motor forward
#define RELAY2_PIN 7 // Left rear motor forward
#define RELAY3_PIN 8 // Right front motor forward
#define RELAY4_PIN 9 // Right rear motor forward

// Define receiver pins
int receiver_pins[] = {A0, A1, A2, A3, A4, A5};
int receiver_values[] = {0, 0, 0, 0, 0, 0};
int res_min = 950;
int res_max = 2020;

int working_range = 255; // motor driver range

boolean prt = true;

int mode = 0;
// -1 = transmitter not connected or out of range
// 0 = transmitter connected and ready
// 1 = slow speed mode
// 2 = high speed mode

void setup() {
  // Set relay pins as output
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(RELAY3_PIN, OUTPUT);
  pinMode(RELAY4_PIN, OUTPUT);

  Serial.begin(115200);
}

void loop() {
  receive();

  int m1 = 0;
  int m2 = 0;

  int rot = receiver_values[0];

  if (mode == 1) {
    m1 = receiver_values[1] / 2 + (rot) / 1.5;
    m2 = receiver_values[1] / 2 - (rot) / 1.5;

  } else if (mode == 2) {
    m1 = receiver_values[1] + rot / 1.75;
    m2 = receiver_values[1] - rot / 1.75;
  }

  // Control relays based on motor values
  controlRelays(m1, m2);
}

int rp = 0;

void receive() {
  receiver_values[rp] = map(pulseIn(receiver_pins[rp], HIGH), res_min, res_max, -1 * working_range, working_range);
  rp++;
  if (rp == 6) {
    rp = 0;
  }
  boolean activevalues = true;
  for (int i = 0; i < 6; i++) {
    if (prt) {
      Serial.print("CH");
      Serial.print(i);
      Serial.print(" : ");
      Serial.print(receiver_values[i]);
      Serial.print(",\t");
    }
    if (receiver_values[i] < -500) {
      activevalues = false;
    }
  }
  mode = 0;
  if (!activevalues) {
    mode = -1;
  } else if (receiver_values[4] > -100) {
    mode = 2;
  } else if (receiver_values[5] > -100) {
    mode = 1;
  }
  if (prt) {
    Serial.println("");
  }
}

void controlRelays(int m1, int m2) {
  if (m1 > 0 && m2 > 0) {
    // Forward
    digitalWrite(RELAY1_PIN, HIGH); // Left front motor forward
    digitalWrite(RELAY2_PIN, HIGH); // Left rear motor forward
    digitalWrite(RELAY3_PIN, HIGH); // Right front motor forward
    digitalWrite(RELAY4_PIN, HIGH); // Right rear motor forward
  } else if (m1 < 0 && m2 < 0) {
    // Backward
    digitalWrite(RELAY1_PIN, LOW); // Left front motor backward
    digitalWrite(RELAY2_PIN, LOW); // Left rear motor backward
    digitalWrite(RELAY3_PIN, LOW); // Right front motor backward
    digitalWrite(RELAY4_PIN, LOW); // Right rear motor backward
  } else if (m1 > 0 && m2 < 0) {
    // Left turn
    digitalWrite(RELAY1_PIN, LOW); // Left front motor backward
    digitalWrite(RELAY2_PIN, LOW); // Left rear motor backward
    digitalWrite(RELAY3_PIN, HIGH); // Right front motor forward
    digitalWrite(RELAY4_PIN, HIGH); // Right rear motor forward
  } else if (m1 < 0 && m2 > 0) {
    // Right turn
    digitalWrite(RELAY1_PIN, HIGH); // Left front motor forward
    digitalWrite(RELAY2_PIN, HIGH); // Left rear motor forward
    digitalWrite(RELAY3_PIN, LOW); // Right front motor backward
    digitalWrite(RELAY4_PIN, LOW); // Right rear motor backward
  } else {
    // Stop
    digitalWrite(RELAY1_PIN, LOW); // Stop left front motor
    digitalWrite(RELAY2_PIN, LOW); // Stop left rear motor
    digitalWrite(RELAY3_PIN, LOW); // Stop right front motor
    digitalWrite(RELAY4_PIN, LOW); // Stop right rear motor
  }
}
