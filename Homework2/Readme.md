# Embedded IoT Homework 2: Dual PWM LED Control with OLED Monitoring

An ESP32 project that fades two LEDs using different PWM frequencies (Task 1)
and then shows the live duty cycle and frequency of each LED on an SSD1306
OLED display (Task 2). It was built and tested in the Wokwi simulator and on
a physical ESP32 board.

## Links
- Wokwi simulation: https://wokwi.com/projects/478705583352034305
- GitHub repository: https://github.com/Laiba-Sohail-08/24-NTU-CS-FL-1042/

## Components
- ESP32 development board
- 2 LEDs
- 2 resistors, 220 ohm (one for each LED)
- SSD1306 128x64 I2C OLED display
- Breadboard and jumper wires

## Task 1: PWM Control of Two LEDs

---

### Description
The ESP32 controls two LEDs using PWM (Pulse Width Modulation). The LEDs fade
in opposite directions: when LED 1 gets brighter, LED 2 gets dimmer, and then
they swap. Each LED uses a different PWM frequency.

### Wiring (Task 1)
| Component | ESP32 pin |
|---|---|
| LED 1 (through 220 ohm resistor) | GPIO 25 |
| LED 2 (through 220 ohm resistor) | GPIO 26 |
| LED cathodes (short legs) | GND |

### PWM settings
| LED | GPIO | Frequency | Resolution | Duty range |
|---|---|---|---|---|
| LED 1 | 25 | 1000 Hz | 8-bit | 0 to 255 |
| LED 2 | 26 | 5000 Hz | 8-bit | 0 to 255 |

---

## Task 2: OLED PWM Monitor

### Description
An SSD1306 OLED is added to the same project. It displays the duty-cycle
percentage and the configured PWM frequency of both LEDs, and updates while
the LEDs fade.

### Wiring (Task 2)
| Component | ESP32 pin |
|---|---|
| LED 1 (through 220 ohm resistor) | GPIO 18 |
| LED 2 (through 220 ohm resistor) | GPIO 19 |
| LED cathodes (short legs) | GND |
| OLED VCC | 3V3 |
| OLED GND | GND |
| OLED SDA | GPIO 21 |
| OLED SCL | GPIO 22 |

The OLED I2C address is `0x3C`.

### PWM settings
| LED | GPIO | Frequency | Resolution | Duty range |
|---|---|---|---|---|
| LED 1 | 18 | 1000 Hz | 8-bit | 0 to 255 |
| LED 2 | 19 | 5000 Hz | 8-bit | 0 to 255 |

### OLED output
The display shows:

    PWM MONITOR
    LED1: <duty %>
    Freq: 1000 Hz
    LED2: <duty %>
    Freq: 5000 Hz

---

## How it works

### PWM and duty cycle
PWM switches a pin ON and OFF very quickly. The **duty cycle** is the
percentage of each cycle that the pin is ON. A higher duty cycle means the LED
is on for longer, so it looks brighter. A lower duty cycle looks dimmer.

### PWM frequency
The **frequency** is how many ON/OFF cycles happen each second. At 1000 Hz and
5000 Hz the switching is too fast for the eye to see, so the LED looks steadily
lit, and its brightness depends on the duty cycle. The two LEDs use different
frequencies to show that each PWM output can be configured independently.

### Duty cycle control
The program uses 8-bit resolution, so the duty value goes from 0 (always off)
to 255 (always on). A loop raises the value from 0 to 255 in steps of 3 and
then lowers it back to 0. LED 1 uses this value directly and LED 2 uses
`255 - value`, which makes them fade in opposite directions. The percentage
shown on the OLED is `duty x 100 / 255`.

### LEDC timer limitation
The ESP32 generates PWM with its LEDC peripheral. Each LEDC timer has one
frequency, and channels that share a timer share that frequency. To use two
different frequencies, the two LEDs must use channels on different timers. This
project uses channels 0 and 2, which use different timers. The program also
prints the real frequency returned by `ledcSetup()` to the serial monitor to
verify the setting. Other ESP32 variants have different timer layouts, so this
should be checked for each chip.

### OLED values match the program
The same variables (`duty1` and `duty2`) are written to the LEDs and drawn on
the OLED, so the display always shows the