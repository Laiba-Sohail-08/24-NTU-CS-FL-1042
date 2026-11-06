
#include <Arduino.h>

#define DAC_PIN 26
#define LOOP_DELAY 20

void setup()
{
    Serial.begin(115200);
    Serial.println("ESP32 DAC2 LED Fading");
}

void loop()
{
    // Increase brightness
    for (int value = 0; value <= 255; value++)
    {
        dacWrite(DAC_PIN, value);
        delay(LOOP_DELAY);
    }

    // Decrease brightness
    for (int value = 255; value >= 0; value--)
    {
        dacWrite(DAC_PIN, value);
        delay(LOOP_DELAY);
    }
}
