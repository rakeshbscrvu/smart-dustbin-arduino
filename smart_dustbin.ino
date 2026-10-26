/*
  Smart Auto-Opening Dustbin with Bin-Full Phone Alert
  Board : Arduino Uno

  Features:
  1. Lid opens automatically when a person comes near (outer ultrasonic sensor + servo)
  2. Fill level is measured by the inner ultrasonic sensor (only while lid is closed)
  3. When the bin is 80% full or more: red LED + buzzer ON and
     "ALERT: Dustbin is FULL!" is sent to the phone over Bluetooth (HC-05) - once
  4. When the bin is emptied (below 50%): green LED ON, alert resets
*/

#include <Servo.h>
#include <SoftwareSerial.h>

// ---------- Pin definitions ----------
const int trigOut  = 9;    // outer sensor TRIG
const int echoOut  = 10;   // outer sensor ECHO
const int trigIn   = 7;    // inner sensor TRIG
const int echoIn   = 8;    // inner sensor ECHO
const int servoPin = 6;    // servo signal
const int buzzer   = 4;    // buzzer +
const int redLED   = 3;    // red LED (bin full)
const int greenLED = 2;    // green LED (bin not full)

// HC-05: Arduino D11 = RX (HC-05 TXD), Arduino D12 = TX (HC-05 RXD via voltage divider)
SoftwareSerial bt(11, 12);
Servo lid;

// ---------- Settings (change these for your bin) ----------
const int BIN_HEIGHT    = 30;   // cm, inner sensor to bin bottom (measure your bin)
const int FULL_LEVEL    = 80;   // % at or above which the bin is "full"
const int RESET_LEVEL   = 50;   // % below which the alert is re-armed
const int OPEN_DISTANCE = 20;   // cm, person/hand detection range
const int LID_OPEN      = 90;   // servo angle when open
const int LID_CLOSED    = 0;    // servo angle when closed

bool alertSent = false;

// Returns distance in cm (999 if no echo)
long getDistance(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long duration = pulseIn(echo, HIGH, 30000);
  if (duration == 0) return 999;
  return duration * 0.034 / 2;
}

void setup() {
  pinMode(trigOut, OUTPUT);
  pinMode(echoOut, INPUT);
  pinMode(trigIn, OUTPUT);
  pinMode(echoIn, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  lid.attach(servoPin);
  lid.write(LID_CLOSED);

  Serial.begin(9600);
  bt.begin(9600);
  Serial.println("Smart Dustbin started");
}

void loop() {
  // 1. Auto-open lid when someone is near
  if (getDistance(trigOut, echoOut) < OPEN_DISTANCE) {
    lid.write(LID_OPEN);
    delay(3000);
    lid.write(LID_CLOSED);
    delay(800);              // let the lid settle before measuring
  }

  // 2. Measure fill level (lid is closed here)
  long dist = getDistance(trigIn, echoIn);
  int fill = (BIN_HEIGHT - dist) * 100 / BIN_HEIGHT;
  fill = constrain(fill, 0, 100);
  Serial.print("Fill level: ");
  Serial.print(fill);
  Serial.println("%");

  // 3. Full alert
  if (fill >= FULL_LEVEL) {
    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);
    digitalWrite(buzzer, HIGH);
    if (!alertSent) {
      bt.println("ALERT: Dustbin is FULL!");
      Serial.println("ALERT: Dustbin is FULL!");
      alertSent = true;      // send only once until emptied
    }
  } else {
    digitalWrite(buzzer, LOW);
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, HIGH);
    if (fill < RESET_LEVEL) {
      alertSent = false;     // bin emptied, ready for next alert
    }
  }

  delay(500);
}
