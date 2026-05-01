/*
  Morse Code Decoder using Push Button and Serial Monitor
  Based on original by Pinaki and Priyanka Sadhukhan
  Optimized and improved for clarity and maintainability by Greenwich Library Innovation Lab
*/

const int inputPin = 2;     // Push button input
const int ledPin = 4;       // Output LED
const int greenPin = 8;       // Output LED
const unsigned int debounceDelay = 75;     // Debounce time in ms
const unsigned int dotThreshold = 500;     // Max duration for a dot
const unsigned int letterPause = 500;      // Pause time to end a letter

unsigned long pressStart = 0;
String morseCode = "";

// Morse Code Lookup Table (A–Z)
const String morseAlphabet[26] = {
  ".-", // A
  "-...", // B
  "-.-.", // C
  "-..", // D
  ".", // E
  "..-.", // F
  "--.", // G
  "....", // H
  "..", // I
  ".---", // J
  "-.-", // K
  ".-..", // L
  "--", // M
  "-.", // N
  "---", // O
  ".--.", // P
  "--.-", // Q
  ".-.", // R
  "...", // S
  "-", // T
  "..-", // U
  "...-", // V
  ".--", // W
  "-..-", // X
  "-.--", // Y
  "--.." // Z
};

void setup() {
  Serial.begin(9600);
  pinMode(inputPin, INPUT_PULLUP); // Use internal pull-up resistor
  pinMode(ledPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
}

void loop() {
  if (press()) {
    pressStart = millis();

  digitalWrite(ledPin, HIGH);
  digitalWrite(greenPin, HIGH);

    // Wait for button release
    while (press()) {
      int t = millis() - pressStart;
       digitalWrite(ledPin, t <= dotThreshold ? HIGH : (t / 100 % 2 == 0 ? LOW : HIGH));
    
    digitalWrite(greenPin, t <= dotThreshold ? HIGH : (t / 100 % 2 == 0 ? LOW : HIGH));}

    digitalWrite(ledPin, LOW);
    digitalWrite(greenPin, LOW);
    unsigned long pressDuration = millis() - pressStart;

    if (pressDuration > debounceDelay) {
      morseCode += decodeSignal(pressDuration);
    }
    else if (morseCode == "") {
      return;
    }

    // Wait to see if more signals follow
    unsigned long pauseStart = millis();

    while (millis() - pauseStart < letterPause) {
      if (press()) return;
    }

    char decodedChar = decodeMorse(morseCode);
    if (decodedChar != '?') {
      Serial.print(decodedChar);
    }
    else {
      Serial.print(" [Unknown: ");
      Serial.print(morseCode);
      Serial.print("] ");
    }
    morseCode = "";
  }
}

// Convert signal duration into dot or dash
char decodeSignal(unsigned long duration) {
  return (duration < dotThreshold) ? '.' : '-';
}

// Decode Morse sequence into a character
char decodeMorse(String code) {
  for (int i = 0; i < 26; i++) {
    if (code == morseAlphabet[i]) {
      return 'A' + i;
    }
  }
  return '?'; // Unknown code
}

bool press() {
   return digitalRead(inputPin) == LOW;
}
