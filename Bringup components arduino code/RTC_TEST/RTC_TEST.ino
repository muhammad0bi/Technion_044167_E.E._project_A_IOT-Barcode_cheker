#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;

const int interruptPin = 12; // Example pin for CLK_INT

void setup() {
  Serial.begin(115200);
  Wire.begin(27, 14); // SDA, SCL
  pinMode(interruptPin, INPUT);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // Uncomment to set the time
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  // Configure the RTC to use the alarm
  rtc.setAlarm1(DateTime(2024, 11, 2, 10, 0, 0), DS3231_A1_Hour); // Set alarm for 10:00:00 on a specific date
  rtc.alarmInterrupt(1, true); // Enable alarm interrupt
}

void loop() {
  if (digitalRead(interruptPin) == HIGH) {
    Serial.println("Alarm triggered!");
    // Clear the alarm
    rtc.clearAlarm(1);
  }

  DateTime now = rtc.now();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  delay(1000);
}
