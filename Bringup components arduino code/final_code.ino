#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>
#include <SD.h>

// Define the pins of the screen for the ESP32
#define SCREEN_CS  32
#define SCREEN_RESET  4
#define SCREEN_DC  5
#define SCREEN_MOSI  23
#define SCREEN_CLK  18
#define SCREEN_SCREEN_ON_OFF  33

// Create an instance of the ILI9341 display
Adafruit_ILI9341 screen = Adafruit_ILI9341(SCREEN_CS, SCREEN_DC, SCREEN_RESET);

// Define the pins of switchs for the ESP32
#define ESP_GOLDEN_SWITCH  36
#define ESP_OPTIONAL_SWITCH  35
#define ESP_WORK_SWITCH  34

// Define the pins of BARCODE for the ESP32
#define ESP_RX_BARCODE_TX  16
#define ESP_TX_BARCODE_RX  17
#define BARCODE_ON_OFF  22

// Create an instance of the HardwareSerial class for the ESP32 BARCODE
HardwareSerial barcode(1);

// Define the pins of real time clock (RTC) for the ESP32
#define RTC_SDA  27
#define RTC_SCL  14

// Create an instance of the RTC
RTC_DS3231 RTC;

// Define the pins of SD card for the ESP32
// #define SD_DO  7
// #define SD_SCLK  6
// #define SD_DI  8
// #define SD_CS  11

// Setup function
void setup() 
{
  // Initialize serial communication for debugging
  Serial.begin(115200);
  Serial.println("Initializing...");

  // Initialize the display
  pinMode(SCREEN_SCREEN_ON_OFF, OUTPUT);
  digitalWrite(SCREEN_SCREEN_ON_OFF,HIGH); // turn the screen on (open the gate in mosfet)
  screen.begin();
  screen.setRotation(3); // Adjust rotation as needed for your display orientation
  screen.fillScreen(ILI9341_WHITE);  // Fill the screen with a color
  screen.setCursor(10, 100);
  screen.setTextColor(ILI9341_BLACK);
  screen.setTextSize(2);
  screen.print("Welcome...");

  // Initialize the pins of switchs
  pinMode(ESP_GOLDEN_SWITCH, INPUT);
  pinMode(ESP_OPTIONAL_SWITCH, INPUT);
  pinMode(ESP_WORK_SWITCH, INPUT);
  
  // Initialize serial communication with the GM810 barcode scanner
  pinMode(BARCODE_ON_OFF, OUTPUT);
  digitalWrite(BARCODE_ON_OFF,HIGH); // turn the barcode on (enable the dc to dc 5V)
  barcode.begin(9600, SERIAL_8N1, ESP_RX_BARCODE_TX, ESP_TX_BARCODE_RX); // RX pin 16, TX pin 17

  // Initialize RTC 
  Wire.begin(RTC_SDA, RTC_SCL); // SDA, SCL for ESP32
  if (!RTC.begin()) 
  {
    Serial.println("Couldn't find RTC");
    while (1); // If RTC is not found, stop the program
  }
  //RTC.adjust(DateTime(2024, 11, 5, 16, 09, 00)); // Set the date to 2024-11-05 12:30:00
  delay(5000);
  screen.fillScreen(ILI9341_WHITE);
  screen.setCursor(10, 100);
  screen.setTextColor(ILI9341_BLACK);
  screen.println("Press to start");
  }

// Function to read barcode data
String readBarcode() 
{
  String barcodeData = "";
  while (1)
  {
    if (barcode.available()) 
    {
      // Read the incoming data
      while (barcode.available()) 
      {
        char c = barcode.read();
        barcodeData += c;
        Serial.println("I'M sacnning");
        delay(20);
      }
      Serial.println("I'M DONE");
      return barcodeData;
    }
  }
  return barcodeData;
}

void loop() 
{
  if (digitalRead(ESP_GOLDEN_SWITCH) == LOW)
  {
    screen.fillScreen(ILI9341_WHITE);
    screen.setCursor(10, 100);
    screen.setTextColor(ILI9341_BLACK);
    screen.print("Scan your golden Barcode");

    // Read the golden barcode
    String golden_barcode = readBarcode();
    Serial.println(golden_barcode);
    if (golden_barcode == "")
    {
      screen.fillScreen(ILI9341_WHITE);  // Clear the screen
      screen.setCursor(10, 100);
      screen.setTextColor(ILI9341_RED);
      screen.print("Timeout: No barcode read");
      return;
    }
    screen.fillScreen(ILI9341_WHITE);  // Clear the screen
    screen.setCursor(10, 100);
    screen.setTextColor(ILI9341_BLACK);
    screen.print("Your golden Barcode: ");
    screen.println(golden_barcode);
    Serial.println(golden_barcode);

    screen.print("Scan your target Barcode");

    // Read the target barcode 
    String target_barcode = readBarcode();
    if (target_barcode == "")
    {
      screen.fillScreen(ILI9341_WHITE);  // Clear the screen
      screen.setCursor(10, 100);
      screen.setTextColor(ILI9341_RED);
      screen.print("Timeout: No barcode read");
      return;
    }
    screen.fillScreen(ILI9341_WHITE);  // Clear the screen
    screen.setCursor(10, 100);
    screen.setTextColor(ILI9341_BLACK);
    screen.print("Your target Barcode: ");
    screen.println(target_barcode);

    // Compare the barcodes and display results
    if (golden_barcode == target_barcode)
    {
      screen.fillScreen(ILI9341_GREEN);  // Fill the screen with a color
      screen.setCursor(10, 100);
      screen.setTextColor(ILI9341_BLACK);
      screen.print("Identical Barcodes");
    }
    else 
    {
      screen.fillScreen(ILI9341_RED);  // Fill the screen with a color
      screen.setCursor(10, 100);
      screen.setTextColor(ILI9341_BLACK);
      screen.print("Different Barcodes");
    }
  }
  if (digitalRead(ESP_OPTIONAL_SWITCH) == LOW)
  {
    //Serial.println("optional_switch pressed");
  }
  if (digitalRead(ESP_WORK_SWITCH) == LOW)
  {
    //Serial.println("work_switch pressed");
  }

}
