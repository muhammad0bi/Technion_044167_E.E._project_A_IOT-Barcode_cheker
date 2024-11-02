#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>
#include <SD.h>

// Define the pins of the screen for the ESP32
#define TFT_CS  32
#define TFT_RESET  4
#define TFT_DC  19
#define TFT_MOSI  23
#define TFT_CLK  13
#define TFT_SCREEN_ON_OFF  33

// Create an instance of the ILI9341 display
Adafruit_ILI9341 screen = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RESET);

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
#define RTC_CLK_INT  12

// Create an instance of the RTC
RTC_DS3231 RTC;


// Define the pins of SD card for the ESP32
// #define SD_DO  7
// #define SD_SCLK  6
// #define SD_DI  8
// #define SD_CS  11

const int SD_CS = 11;

// Setup function
void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);
  Serial.println("Initializing...");

  //Initialize the RTC pins
  Wire.begin(RTC_SDA, RTC_SCL); // SDA 27 , SCL 14
  pinMode(RTC_CLK_INT, INPUT); // CLK_INT 12

  // Initialize the display
  screen.begin();

  // Initialize serial communication with the GM810 barcode scanner through TTL-232
  // Ensure the baud rate matches the GM810 scanner's setting
  barcode.begin(9600, SERIAL_8N1, ESP_RX_BARCODE_TX, ESP_TX_BARCODE_RX); // RX pin 16, TX pin 17

  // Initialize the SD card
  if (!SD.begin(SD_CS)) 
  {
    Serial.println("SD Initialization failed!");
    return;
  }

}

void loop() {
  // put your main code here, to run repeatedly:

}
