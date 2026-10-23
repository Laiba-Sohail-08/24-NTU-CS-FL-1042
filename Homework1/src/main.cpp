#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// screen settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDRESS 0x3C
#define SDA_PIN 21
#define SCL_PIN 22
#define BUTTON_PIN 18

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// variables for button and counter
int pressCount = 0;
int lastReading = HIGH;
int stableState = HIGH;
unsigned long lastChangeTime = 0;
const unsigned long debounceDelay = 50;

// function to show the output on oled
void updateDisplay() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("ESP32 Button Counter");

  display.setCursor(0, 20);
  if (stableState == LOW) {
    display.println("Button: PRESSED");
  } else {
    display.println("Button: RELEASED");
  }

  display.setTextSize(2);
  display.setCursor(0, 42);
  display.print("Count:");
  display.print(pressCount);

  display.display(); // show the output on screen
}

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP); // button is low when pressed
  Wire.begin(SDA_PIN, SCL_PIN);

  // check if the oled is connected properly
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("OLED not found! Check wiring.");
    while (true) {
    }
  }

  Serial.println("System started. Count = 0");
  updateDisplay(); // show the starting output
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);

  // check if the button reading has changed
  if (reading != lastReading) {
    lastChangeTime = millis();
    lastReading = reading;
  }

  // wait for the button reading to stay stable for 50 ms
  if ((millis() - lastChangeTime) > debounceDelay) {

    // check if the button state has really changed
    if (reading != stableState) {
      stableState = reading;

      // increase count when the button is pressed
      if (stableState == LOW) {
        pressCount++;
        Serial.print("Press accepted! Count = ");
        Serial.println(pressCount);
      }

      updateDisplay(); // refresh the screen
    }
  }
}