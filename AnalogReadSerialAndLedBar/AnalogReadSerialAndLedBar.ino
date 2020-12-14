/*
 * AnalogReadSerialAndLedBar
 *
 * Read analog signal from A0 and show the level
 * of it on numberic or led-line indicator
*/
const int MAX_VALUE = 1023; // Maximum value of analog signal after DAC processing
const int RANGE_COUNT = 9;  // Count of LED in line
const int FIRST_PIN = 3;    // First pin for the first LED in line
const int DELAY = 500;      // Period between the digital-analog convertion

const int SCALE_LED = 0;    // Mode for the indicator based on line of LEDs
const int NUMBR_LED = 1;    // Mode for the numberic LED indicator

int mode = SCALE_LED;       // Default mode
int lastTime = 0;           // Time since the start of the power supply

/*
 * Board setup
 */
void setup() {
  initPins(RANGE_COUNT, FIRST_PIN);
  attachInterrupt(0, changeMode, FALLING);
  Serial.begin(9600);
}

/*
 * Main cycle
 */
void loop() {
  if (tick()) {
    int sensorValue = analogRead(A0);
    switch(mode) {
      case SCALE_LED:
        changeScaleIndicationState(sensorValue, MAX_VALUE, RANGE_COUNT);
        break;
      case NUMBR_LED:
        changeNumberIndicationState(sensorValue, MAX_VALUE);
        break;
    } 
  }
}

/*
 * Returns true value each period (due to DELAY constant)
 */
bool tick() {
  int currentTime = millis();
  if (currentTime - DELAY >= lastTime) {
    lastTime = currentTime;
    return true;
  }
  return false;
}

/*
 * Initialization of all ports
 */
void initPins(int count, int firstPin) {
  pinMode(FIRST_PIN - 1, INPUT_PULLUP);
  for (int i = 0; i < count; i++) {
    pinMode(firstPin + i, OUTPUT);
    digitalWrite(i, LOW);
  }
}

/*
 * Changed the mode (NUMBR_LED / SCALE_LED)
 */
void changeMode() {
  if (mode == SCALE_LED) {
    mode = NUMBR_LED;
  } else {
    mode = SCALE_LED;
  }
  Serial.println(mode);
}

/*
 * Changes the showing value as a scale (due to maximum RANGE_COUNT)
 */
void changeScaleIndicationState(int value, int maxValue, int rangeCount) {
  float dv = (float) maxValue / (float) rangeCount;
  for (int i = 0; i < rangeCount; i++) {
    if ((float) value >= dv * (float) i) {
      digitalWrite(FIRST_PIN + i, HIGH);
    } else {
      digitalWrite(FIRST_PIN + i, LOW);
    }
  }
  Serial.println(value * 5.0 / maxValue);
}

/*
 * Changes the showing value as a number (from 0 to 9)
 */
void changeNumberIndicationState(int value, int maxValue) {
  int NUMBER_MAX = 10;
  float dv = (float) maxValue / (float) NUMBER_MAX;
  for (int i = 0; i < NUMBER_MAX; i++) {
    if ((float) value >= dv * (float) i) {
      setNumber(i);
    }
  }
  Serial.println(value * 5.0 / maxValue);
}

/*
 * Decodes number to the pins' mode
 */
void setNumber(int number) {
  switch(number) {
    case 0: // Number is 0
      digitalWrite(FIRST_PIN + 0, HIGH);
      digitalWrite(FIRST_PIN + 1, HIGH);
      digitalWrite(FIRST_PIN + 2, HIGH);
      digitalWrite(FIRST_PIN + 3, LOW);
      digitalWrite(FIRST_PIN + 4, HIGH);
      digitalWrite(FIRST_PIN + 5, HIGH);
      digitalWrite(FIRST_PIN + 6, HIGH);
      break;
    case 1: // Number is 1
      digitalWrite(FIRST_PIN + 0, LOW);
      digitalWrite(FIRST_PIN + 1, LOW);
      digitalWrite(FIRST_PIN + 2, HIGH);
      digitalWrite(FIRST_PIN + 3, LOW);
      digitalWrite(FIRST_PIN + 4, LOW);
      digitalWrite(FIRST_PIN + 5, LOW);
      digitalWrite(FIRST_PIN + 6, HIGH);
      break;
    case 2: // Number is 2
      digitalWrite(FIRST_PIN + 0, HIGH);
      digitalWrite(FIRST_PIN + 1, HIGH);
      digitalWrite(FIRST_PIN + 2, LOW);
      digitalWrite(FIRST_PIN + 3, HIGH);
      digitalWrite(FIRST_PIN + 4, LOW);
      digitalWrite(FIRST_PIN + 5, HIGH);
      digitalWrite(FIRST_PIN + 6, HIGH);
      break;
    case 3: // Number is 3
      digitalWrite(FIRST_PIN + 0, LOW);
      digitalWrite(FIRST_PIN + 1, HIGH);
      digitalWrite(FIRST_PIN + 2, HIGH);
      digitalWrite(FIRST_PIN + 3, HIGH);
      digitalWrite(FIRST_PIN + 4, LOW);
      digitalWrite(FIRST_PIN + 5, HIGH);
      digitalWrite(FIRST_PIN + 6, HIGH);
      break;
    case 4: // Number is 4
      digitalWrite(FIRST_PIN + 0, LOW);
      digitalWrite(FIRST_PIN + 1, LOW);
      digitalWrite(FIRST_PIN + 2, HIGH);
      digitalWrite(FIRST_PIN + 3, HIGH);
      digitalWrite(FIRST_PIN + 4, HIGH);
      digitalWrite(FIRST_PIN + 5, LOW);
      digitalWrite(FIRST_PIN + 6, HIGH);
      break;
    case 5: // Number is 5
      digitalWrite(FIRST_PIN + 0, LOW);
      digitalWrite(FIRST_PIN + 1, HIGH);
      digitalWrite(FIRST_PIN + 2, HIGH);
      digitalWrite(FIRST_PIN + 3, HIGH);
      digitalWrite(FIRST_PIN + 4, HIGH);
      digitalWrite(FIRST_PIN + 5, HIGH);
      digitalWrite(FIRST_PIN + 6, LOW);
      break;
    case 6: // Number is 6
      digitalWrite(FIRST_PIN + 0, HIGH);
      digitalWrite(FIRST_PIN + 1, HIGH);
      digitalWrite(FIRST_PIN + 2, HIGH);
      digitalWrite(FIRST_PIN + 3, HIGH);
      digitalWrite(FIRST_PIN + 4, HIGH);
      digitalWrite(FIRST_PIN + 5, HIGH);
      digitalWrite(FIRST_PIN + 6, LOW);
      break;
    case 7: // Number is 7
      digitalWrite(FIRST_PIN + 0, LOW);
      digitalWrite(FIRST_PIN + 1, LOW);
      digitalWrite(FIRST_PIN + 2, HIGH);
      digitalWrite(FIRST_PIN + 3, LOW);
      digitalWrite(FIRST_PIN + 4, LOW);
      digitalWrite(FIRST_PIN + 5, HIGH);
      digitalWrite(FIRST_PIN + 6, HIGH);
      break;
    case 8: // Number is 8
      digitalWrite(FIRST_PIN + 0, HIGH);
      digitalWrite(FIRST_PIN + 1, HIGH);
      digitalWrite(FIRST_PIN + 2, HIGH);
      digitalWrite(FIRST_PIN + 3, HIGH);
      digitalWrite(FIRST_PIN + 4, HIGH);
      digitalWrite(FIRST_PIN + 5, HIGH);
      digitalWrite(FIRST_PIN + 6, HIGH);
      break;
    case 9: // Number is 9
      digitalWrite(FIRST_PIN + 0, LOW);
      digitalWrite(FIRST_PIN + 1, HIGH);
      digitalWrite(FIRST_PIN + 2, HIGH);
      digitalWrite(FIRST_PIN + 3, HIGH);
      digitalWrite(FIRST_PIN + 4, HIGH);
      digitalWrite(FIRST_PIN + 5, HIGH);
      digitalWrite(FIRST_PIN + 6, HIGH);
      break;
  }
}
