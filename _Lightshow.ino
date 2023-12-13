#include <CapacitiveSensor.h>

#define rgbRed 9
#define rgbGreen 10
#define rgbBlue 11
#define buttonPin 7
#define photocellPin A0
#define touchSensorPin 2
#define ledPin2 2  
#define ledPin3 3  
#define speakerPin 12  

int jingleBellsMelody[] = {
  659, 659, 659,
  659, 659, 659,
  659, 784, 523, 587, 659,
  698, 698, 698, 698,
  698, 659, 659, 659, 659,
  659, 587, 587, 659,
  587, 784
};

int jingleBellsDurations[] = {
  200, 200, 400,
  200, 200, 400,
  200, 200, 200, 200, 200,
  400, 200, 200, 200,
  200, 200, 200, 200, 200,
  200, 200, 200, 400,
  400, 400
};



unsigned long lastButtonPressTime = 0;
unsigned long buttonPressInterval = 250; // Adjust this interval as needed
unsigned long lastPhotocellUpdateTime = 0;
unsigned long photocellUpdateInterval = 100; // Adjust this interval as needed
unsigned long lastCapacitiveSensorTime = 0;
unsigned long capacitiveSensorInterval = 100; // Adjust this interval as needed

int randomRed, randomGreen, randomBlue;
int photocellBrightness;

CapacitiveSensor cs_9_8 = CapacitiveSensor(5, 4);

void setup() {
  pinMode(rgbRed, OUTPUT);
  pinMode(rgbGreen, OUTPUT);
  pinMode(rgbBlue, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(touchSensorPin, INPUT_PULLUP);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  Serial.begin(9600);
  randomSeed(analogRead(0));
  cs_9_8.set_CS_AutocaL_Millis(0xFFFFFFFF);
  pinMode(speakerPin, OUTPUT);
}

void loop() {
  handleButtonPress();
  updatePhotocellBrightness();
  handleCapacitiveSensor();
}

void handleButtonPress() {
  int buttonState = digitalRead(buttonPin);
  if (buttonState == LOW && millis() - lastButtonPressTime > buttonPressInterval) {
    changeColorRandomly();
    lastButtonPressTime = millis();
  }
}

void updatePhotocellBrightness() {
  if (millis() - lastPhotocellUpdateTime > photocellUpdateInterval) {
    int photocellValue = analogRead(photocellPin);
    photocellBrightness = map(photocellValue, 550, 1023, 0, 255);
    setRGBBrightness(photocellBrightness);
    printPhotocellValues(photocellValue);
    lastPhotocellUpdateTime = millis();
  }
}

void setRGBBrightness(int brightness) {
  analogWrite(rgbRed, brightness);
  analogWrite(rgbGreen, brightness);
  analogWrite(rgbBlue, brightness);
}

void printPhotocellValues(int value) {
  Serial.print("Photocell Value: ");
  Serial.println(value);
}

void changeColorRandomly() {
  randomRed = random(256);
  randomGreen = random(256);
  randomBlue = random(256);

  Serial.println("Button Pressed - Changing Color");
  printRGBValues();
  setRGBColor(randomRed, randomGreen, randomBlue);
}

void printRGBValues() {
  Serial.print("Random Red: ");
  Serial.println(randomRed);
  Serial.print("Random Green: ");
  Serial.println(randomGreen);
  Serial.print("Random Blue: ");
  Serial.println(randomBlue);
}

void setRGBColor(int red, int green, int blue) {
  analogWrite(rgbRed, red);
  analogWrite(rgbGreen, green);
  analogWrite(rgbBlue, blue);
}

void handleCapacitiveSensor() {
  if (millis() - lastCapacitiveSensorTime > capacitiveSensorInterval) {
    long total1 = cs_9_8.capacitiveSensor(30);
    Serial.println("Sensor Value: " + String(total1));

    int threshold = 100;
    if (total1 < threshold) {
      digitalWrite(ledPin2, HIGH);
      digitalWrite(ledPin3, HIGH);
      playMelody(jingleBellsMelody, jingleBellsDurations, sizeof(jingleBellsMelody) / sizeof(jingleBellsMelody[0]));
    } else {
      digitalWrite(ledPin2, LOW);
      digitalWrite(ledPin3, LOW);
    }

    lastCapacitiveSensorTime = millis();
  }
}

void playMelody(int melody[], int durations[], int length) {
  for (int i = 0; i < length; ++i) {
    int noteDuration = 1000 / durations[i];
    tone(speakerPin, melody[i], noteDuration);

    // To distinguish the notes, set a minimum time between them.
    // The default is 10 milliseconds, but you can adjust this.
    delay(noteDuration * 1.1);

    // Stop the tone playing:
    noTone(speakerPin);
  }
}