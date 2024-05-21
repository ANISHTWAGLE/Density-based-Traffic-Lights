const int ledPin = 7;   // the arterial road signal 
const int ledPin2 = 8;  // side road red signal
const int ledPin3 = 9;  // main road green signal

bool ledOn = false;
bool led1On = false;  // Flag to track the state of ledPin1
unsigned long previousMillis = 0;
long interval = 0;

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  pinMode(ledPin, OUTPUT);  // Set the LED pin as an output
  pinMode(ledPin2, OUTPUT); // Set the second LED pin as an output
  pinMode(ledPin3, OUTPUT); // Set the third LED pin as an output
  
  digitalWrite(ledPin2, HIGH);  // Turn on ledPin2 from the start
  digitalWrite(ledPin3, HIGH);  // Turn on ledPin3 from the start
}

void loop() {
  if (Serial.available() > 0) {
    char val = Serial.read();  // Read serial input

    // If '1' is received, turn on the LED and remember the time for 5 seconds
    if (val == '1') {
      digitalWrite(ledPin, HIGH);
      digitalWrite(ledPin2, LOW);  // Turn off ledPin2
      digitalWrite(ledPin3, LOW);  // Turn on ledPin3
      ledOn = true;
      led1On = true;  // Set the flag for ledPin1
      interval = 5000;  // 5 seconds
      previousMillis = millis();
    }
    // If '2' is received, turn on the LED and remember the time for 15 seconds
    else if (val == '2') {
      digitalWrite(ledPin, HIGH);
      digitalWrite(ledPin2, LOW);  // Turn off ledPin2
      digitalWrite(ledPin3, LOW);  // Turn on ledPin3
      ledOn = true;
      led1On = false;  // Reset the flag for ledPin1
      interval = 15000;  // 15 seconds
      previousMillis = millis();
    }
  }

  // Check if the LED has been on for the specified interval
  if (ledOn && millis() - previousMillis >= interval) {
    digitalWrite(ledPin, LOW);  // Turn off the LED
    digitalWrite(ledPin2, HIGH);  // Turn on ledPin2
    digitalWrite(ledPin3, HIGH);  // Turn off ledPin3 if ledPin1 is on
    ledOn = false;  // Reset the flag
  }
}
