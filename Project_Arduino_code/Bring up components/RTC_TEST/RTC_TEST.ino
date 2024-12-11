#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;

void setup() {
  Serial.begin(115200);

  // Initialize I2C with custom SDA and SCL pins (if needed)
  Wire.begin(27, 14); // SDA, SCL for ESP32

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1); // If RTC is not found, stop the program
  }

  // Uncomment to set the time (this sets the time based on the compilation date and time)
  //rtc.adjust(DateTime(2024, 11, 5, 16, 09, 00)); // Set the date to 2024-11-05 12:30:00

}

void loop() {
  // Get current time from RTC
  DateTime now = rtc.now();
  
  // Print current time to serial monitor
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
  
  // Wait for 1 second before printing again
  delay(1000);
}
