#include <Arduino.h>

// LED GPIO pins
const int LED1 = 18;
const int LED2 = 19;

// Separate LEDC channels
const int CHANNEL1 = 0;
const int CHANNEL2 = 2;

// Different PWM frequencies
const int FREQ1 = 1000;
const int FREQ2 = 5000;
const int RESOLUTION = 8;

// Duty-cycle values: 0 to 255
int duty1 = 0;
int duty2 = 0;

void setup()
{
    Serial.begin(115200);

    double actualFreq1 = ledcSetup(CHANNEL1, FREQ1, RESOLUTION);
    ledcAttachPin(LED1, CHANNEL1);

    double actualFreq2 = ledcSetup(CHANNEL2, FREQ2, RESOLUTION);
    ledcAttachPin(LED2, CHANNEL2);

    Serial.printf("LED1 configured frequency: %.0f Hz\n", actualFreq1);
    Serial.printf("LED2 configured frequency: %.0f Hz\n", actualFreq2);
}

void loop()
{
    // Fade LED 1 up and LED 2 down
    for (int duty = 0; duty <= 255; duty++)
    {
        duty1 = duty;
        duty2 = 255 - duty;

        ledcWrite(CHANNEL1, duty1);
        ledcWrite(CHANNEL2, duty2);

        if (duty % 25 == 0)
        {
            Serial.printf("LED1 duty: %d, LED2 duty: %d\n",
                          duty1, duty2);
        }

        delay(10);
    }

    // Fade LED 1 down and LED 2 up
    for (int duty = 255; duty >= 0; duty--)
    {
        duty1 = duty;
        duty2 = 255 - duty;

        ledcWrite(CHANNEL1, duty1);
        ledcWrite(CHANNEL2, duty2);

        delay(10);
    }
}