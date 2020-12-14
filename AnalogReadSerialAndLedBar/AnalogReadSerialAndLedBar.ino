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
  int ledBar = B0000000;
  switch(number) {
    case 0: ledBar = B1110111; break; // Number is 0
    case 1: ledBar = B1000100; break; // Number is 1
    case 2: ledBar = B1101011; break; // Number is 2
    case 3: ledBar = B1101110; break; // Number is 3
    case 4: ledBar = B1011100; break; // Number is 4
    case 5: ledBar = B0111110; break; // Number is 5
    case 6: ledBar = B0111111; break; // Number is 6
    case 7: ledBar = B1100100; break; // Number is 7
    case 8: ledBar = B1111111; break; // Number is 8
    case 9: ledBar = B1111110; break; // Number is 9
  }
  for (int i = 0; i < 8; i++) {
    digitalWrite(FIRST_PIN + i, bitRead(ledBar, i));
  }
}
