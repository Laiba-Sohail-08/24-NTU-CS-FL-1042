
#include <Arduino.h>

#define BUZZER_PIN 21

bool stopRequested = false;

bool waitWithStop(int duration)
{
  for (int i = 0; i < duration; i += 10)
  {
    if (Serial.available())
    {
      char c = Serial.read();

      if (c == '0')
      {
        stopRequested = true;
        digitalWrite(BUZZER_PIN, LOW);
        return false;
      }
    }

    delay(10);
  }

  return true;
}

void playBeep(int duration)
{
  if (stopRequested)
    return;

  digitalWrite(BUZZER_PIN, HIGH);

  if (!waitWithStop(duration))
    return;

  digitalWrite(BUZZER_PIN, LOW);
  waitWithStop(100);
}

void toneSequence()
{
  playBeep(200);
  playBeep(300);
  playBeep(400);
  playBeep(500);
  playBeep(600);
}

void alarmSound()
{
  for (int i = 0; i < 5 && !stopRequested; i++)
  {
    playBeep(300);
    playBeep(300);
  }
}

void doorbell()
{
  for (int i = 0; i < 2 && !stopRequested; i++)
  {
    playBeep(300);
    playBeep(600);
    waitWithStop(300);
  }
}

void shortBeeps()
{
  for (int i = 0; i < 5 && !stopRequested; i++)
  {
    playBeep(100);
    waitWithStop(100);
  }
}

void melody()
{
  int durations[] = {
      100, 150, 200, 250,
      300, 350, 400, 500
  };

  for (int i = 0; i < 8 && !stopRequested; i++)
  {
    playBeep(durations[i]);
  }
}

void showMenu()
{
  Serial.println();
  Serial.println("===== BUZZER MENU =====");
  Serial.println("1. Rising Beep Sequence");
  Serial.println("2. Alarm");
  Serial.println("3. Doorbell");
  Serial.println("4. Short Beeps");
  Serial.println("5. Beep Pattern");
  Serial.println("0. Stop Sound");
  Serial.println("Enter your choice:");
}

void setup()
{
  Serial.begin(115200);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.println("ESP32 Buzzer Ready!");
  showMenu();
}

void loop()
{
  if (Serial.available())
  {
    char choice = Serial.read();

    if (choice == '\n' || choice == '\r')
      return;

    if (choice == '0')
    {
      stopRequested = true;
      digitalWrite(BUZZER_PIN, LOW);
      Serial.println("Buzzer stopped.");
      showMenu();
      return;
    }

    stopRequested = false;

    switch (choice)
    {
      case '1':
        Serial.println("Playing Rising Beep Sequence");
        toneSequence();
        break;

      case '2':
        Serial.println("Playing Alarm");
        alarmSound();
        break;

      case '3':
        Serial.println("Playing Doorbell");
        doorbell();
        break;

      case '4':
        Serial.println("Playing Short Beeps");
        shortBeeps();
        break;

      case '5':
        Serial.println("Playing Beep Pattern");
        melody();
        break;

      default:
        Serial.println("Invalid choice. Enter 0-5.");
        showMenu();
        return;
    }

    digitalWrite(BUZZER_PIN, LOW);

    if (!stopRequested)
      Serial.println("Pattern finished.");

    showMenu();
  }
}