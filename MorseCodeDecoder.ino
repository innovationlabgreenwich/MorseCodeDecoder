/*
  Morse Code Decoder using Push Button and Serial Monitor
  Based on original by Pinaki and Priyanka Sadhukhan
  Optimized and improved for clarity and maintainability by Greenwich Library Innovation Lab
*/

const int inputPin = 2;     // Push button input
const int ledPin = 4;       // Output LED
const unsigned int debounceDelay = 125;     // Debounce time in ms
const unsigned int dotThreshold = 500;     // Max duration for a dot
const unsigned int letterPause = 500;      // Pause time to end a letter

unsigned long pressStart = 0;
String morseCode = "";

// Morse Code Lookup Table (A–Z)
const String morseAlphabet[26] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
  "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
  "..-", "...-", ".--", "-..-", "-.--", "--.."
};

void setup() {
  Serial.begin(9600);
  pinMode(inputPin, INPUT_PULLUP); // Use internal pull-up resistor
  pinMode(ledPin, OUTPUT);
}

void loop() {
  if (digitalRead(inputPin) == LOW) {
    pressStart = millis();
    digitalWrite(ledPin, HIGH);

    // Wait for button release
    while (digitalRead(inputPin) == LOW) {}

    digitalWrite(ledPin, LOW);
    unsigned long pressDuration = millis() - pressStart;

    if (pressDuration > debounceDelay) {
      morseCode += decodeSignal(pressDuration);
    }

    // Wait to see if more signals follow
    unsigned long pauseStart = millis();

    while (millis() - pauseStart < letterPause) {
      if (digitalRead(inputPin) == LOW) return;
    }

    char decodedChar = decodeMorse(morseCode);
    if (decodedChar != '?') {
      Serial.print(decodedChar);
    } else {
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
