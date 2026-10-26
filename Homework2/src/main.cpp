#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// LED GPIO pins
const int LED1 = 18;
const int LED2 = 19;

// Separate LEDC channels
const int CHANNEL1 = 0;
const int CHANNEL2 = 2;

// PWM settings
const int FREQ1 = 1000;
const int FREQ2 = 5000;
const int RESOLUTION = 8;

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

Adafruit_SSD1306 display(
    SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Duty-cycle values: 0 to 255
int duty1 = 0;
int duty2 = 255;

void updateOLED()
{
    int percent1 = (duty1 * 100) / 255;
    int percent2 = (duty2 * 100) / 255;

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 0);
    display.println("PWM MONITOR");

    display.setCursor(0, 18);
    display.print("LED1: ");
    display.print(percent1);
    display.print("%");

    display.setCursor(0, 28);
    display.print("Freq: ");
    display.print(FREQ1);
    display.println(" Hz");

    display.setCursor(0, 42);
    display.print("LED2: ");
    display.print(percent2);
    display.print("%");

    display.setCursor(0, 52);
    display.print("Freq: ");
    display.print(FREQ2);
    display.println(" Hz");

    display.display();
}

void setup()
{
    Serial.begin(115200);

    // Start I2C: SDA = GPIO 21, SCL = GPIO 22
    Wire.begin(21, 22);
    Wire.setClock(400000);   // faster I2C so the OLED updates quickly

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS))
    {
        Serial.println("OLED initialization failed!");
        while (true)
        {
            delay(100);
        }
    }

    display.clearDisplay();
    updateOLED();

    double actualFreq1 =
        ledcSetup(CHANNEL1, FREQ1, RESOLUTION);
    ledcAttachPin(LED1, CHANNEL1);

    double actualFreq2 =
        ledcSetup(CHANNEL2, FREQ2, RESOLUTION);
    ledcAttachPin(LED2, CHANNEL2);

    Serial.printf("LED1 configured frequency: %.0f Hz\n",
                  actualFreq1);
    Serial.printf("LED2 configured frequency: %.0f Hz\n",
                  actualFreq2);
}

void loop()
{
    // LED1 fades up; LED2 fades down
    for (int duty = 0; duty <= 255; duty += 3)
    {
        duty1 = duty;
        duty2 = 255 - duty;

        ledcWrite(CHANNEL1, duty1);
        ledcWrite(CHANNEL2, duty2);

        updateOLED();

        delay(10);
    }

    // LED1 fades down; LED2 fades up
    for (int duty = 255; duty >= 0; duty -= 3)
    {
        duty1 = duty;
        duty2 = 255 - duty;

        ledcWrite(CHANNEL1, duty1);
        ledcWrite(CHANNEL2, duty2);

        updateOLED();

        delay(10);
    }
}