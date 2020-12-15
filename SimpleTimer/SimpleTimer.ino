
/*
 * SimpleTimer
 *
 * For showing the surrent timer value is used
 * just the stack of 4 7-segments indicators
*/
const int PIN_COUNT = 12;   // Count of pins
const int DIGIT_COUNT = 4;  // Count of digits
const int FIRST_PIN = 2;    // First pin
const int DELAY = 5;        // Period between screen updating
const int INTERVAL = 1000;  // One second

unsigned long lastTime = 0; // Time since the start of the power supply
unsigned long timerVal = 0; // Timer value in seconds
int currentDigit = 0;       // Current digit
bool isDotUp = true;        // Dot between minutes and seconds

/*
 * Board setup
 */
void setup() {
  initPins(PIN_COUNT, FIRST_PIN);
  Serial.begin(9600);
}

/*
 * Main cycle
 */
void loop() {
  if (tick(DELAY)) {
    selectPin(FIRST_PIN, PIN_COUNT, DIGIT_COUNT, currentDigit);
    nextDigit(DIGIT_COUNT);
    showCurrentTimeDigit(currentDigit, timerVal, currentDigit == 2 && isDotUp);
  }
  timerTick(INTERVAL);
}

/*
 * Initialization of all ports
 */
void initPins(int count, int firstPin) {
  for (int i = 0; i < count; i++) {
    pinMode(firstPin + i, OUTPUT);
    digitalWrite(firstPin + i, LOW);
  }
}

/*
 * Returns true value each period (due to DELAY constant)
 */
bool tick(int t) {
  int currentTime = millis();
  if (currentTime - t >= lastTime) {
    lastTime = currentTime;
    return true;
  }
  return false;
}

/*
 * Set new value of the timer each period (due to INTERVAL constant)
 */
void timerTick(int t) {
  unsigned long currentTime = millis();
  if (currentTime - t >= timerVal * t) {
    timerVal = floor((float) currentTime / (float) t);
    if (timerVal > 60 * 60 * 9) { // magic value from expirements
      timerVal = 1;
    }
    isDotUp = !isDotUp;
  }
}

/*
 * Shows digit due to order
 */
void showCurrentTimeDigit(int digitNumber, unsigned long currentSeconds, bool isWithDot) {
  unsigned long currentDigitValue = currentSeconds;
  switch(digitNumber) {
    case 1:
      currentDigitValue = (unsigned long) floor((float) currentDigitValue / 600.0) - (21600 * (int) floor((float) currentDigitValue / 216000.0));
      currentDigitValue = currentDigitValue - 6 * (currentDigitValue / 6);
      break;
    case 2:
      currentDigitValue = (unsigned long) ((float) currentDigitValue / 60.0) - (3600 * (int) floor((float) currentDigitValue / 36000.0));
      currentDigitValue = currentDigitValue - 10 * (currentDigitValue / 10);
      break;
    case 3:
      currentDigitValue = (unsigned long) ((float) currentDigitValue / 10.0) - (60 * (int) floor((float) currentDigitValue / 600.0));
      currentDigitValue = currentDigitValue - 6 * (currentDigitValue / 6);
      break;
    case 4:
      currentDigitValue -= 10 * (unsigned long) floor((float) currentDigitValue / 10.0);
      break;
  }
  setNumber(currentDigitValue, FIRST_PIN, isWithDot);
}

/*
 * Set high level on all digits' ground
 */
void selectPin(int firstPin, int pinCount, int digitCount, int currentValue) {
  for (int i = firstPin + pinCount - digitCount; i < firstPin + pinCount; i++) {
    digitalWrite(i, HIGH);
  }
  digitalWrite(FIRST_PIN + PIN_COUNT - DIGIT_COUNT + currentValue, LOW);
}

/*
 * Switch next digit
 */
void nextDigit(int digitCount) {
  currentDigit++;
  if (currentDigit > digitCount) {
    currentDigit = 0;
  }
}

/*
 * Decodes number to the pins' mode
 */
void setNumber(unsigned long number, int firstPin, bool isDelemiter) {
  byte ledBar = B00000000;
  switch(number) {
    case 0: ledBar = B11100111; break; // Number is 0
    case 1: ledBar = B10000100; break; // Number is 1
    case 2: ledBar = B11010011; break; // Number is 2
    case 3: ledBar = B11010110; break; // Number is 3
    case 4: ledBar = B10110100; break; // Number is 4
    case 5: ledBar = B01110110; break; // Number is 5
    case 6: ledBar = B01110111; break; // Number is 6
    case 7: ledBar = B11000100; break; // Number is 7
    case 8: ledBar = B11110111; break; // Number is 8
    case 9: ledBar = B11110110; break; // Number is 9
  }
  bitWrite(ledBar, 3, isDelemiter ? 1 : 0);
  for (int i = 0; i < 8; i++) {
    digitalWrite(firstPin + i, bitRead(ledBar, i));
  }
}
