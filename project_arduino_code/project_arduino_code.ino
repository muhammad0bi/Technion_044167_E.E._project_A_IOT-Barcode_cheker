#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// Define the screen pins
#define SCREEN_CS  32
#define SCREEN_RESET  4
#define SCREEN_DC  5
#define SCREEN_MOSI  23
#define SCREEN_CLK  18
#define SCREEN_SCREEN_ON_OFF  33

// Create an instance of the ILI9341 display
Adafruit_ILI9341 screen = Adafruit_ILI9341(SCREEN_CS, SCREEN_DC, SCREEN_RESET);

// Define the switch pin
#define ESP_SWITCH  36

// Define the BARCODE pins
#define ESP_RX_BARCODE_TX  16
#define ESP_TX_BARCODE_RX  17
#define BARCODE_ON_OFF  22

// Create an instance of the HardwareSerial class for the ESP32 BARCODE
HardwareSerial barcode(1);

// Define the real time clock (RTC) pins
#define RTC_SDA  27
#define RTC_SCL  14

// Create an instance of the RTC
RTC_DS3231 RTC;

// Define the SD card plot pins
// #define SD_DO  7
// #define SD_SCLK  6
// #define SD_DI  8
// #define SD_CS  11

//Define the WI-FI 
const char *ssid = "ESP32-Access-Point";  // SSID for the AP
const char *password = "123456789";       // Password for the AP

AsyncWebServer server(80);  // Create a web server on port 80

String golden_barcode = ""; 
String target_barcode = "";
String page = "";
bool update_page = false;
bool golden_mode = false;

// Define the blue LED pin
#define LED_PIN  21

// Define the battry feedback pin
#define BAT_FEEDBACK_PIN  25

//wifi page
void refresh_page()
{
  if (update_page)
  {
    // Start the HTML page with the necessary headers
    page = "<!DOCTYPE html>";
    page += "<html lang='en'>";
    page += "<head>";
    page += "<meta charset='UTF-8'>";
    page += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    page += "<title>Barcode and Date Information</title>";
    page += "<style>";
    page += "body { font-family: Arial, sans-serif; margin: 20px; }";
    page += "h1 { color: #4CAF50; }";
    page += "p { font-size: 16px; }";
    page += "</style>";
    page += "</head>";
    page += "<body>";
    // Display the barcode information
    page += "<p><b>Golden Barcode: </b>" + golden_barcode + "</p>";
    page += "<p><b>Target Barcode: </b>" + target_barcode + "</p>";
    // Compare the barcodes
    if (golden_barcode == target_barcode) 
    {
      page += "<p><b>Comparison: </b>Identical</p>";
    } 
    else
    {
      page += "<p><b>Comparison: </b>Different</p>";
    }
    // **Get the current date and time from the RTC**
    DateTime now = RTC.now();
    char dateBuffer[20];
    snprintf(dateBuffer, sizeof(dateBuffer), "%02d/%02d/%04d", now.day(), now.month(), now.year());
    char timeBuffer[10];
    snprintf(timeBuffer, sizeof(timeBuffer), "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
    // Display RTC information on the page
    page += "<p><b>Date: </b>" + String(dateBuffer) + "</p>";
    page += "<p><b>Time: </b>" + String(timeBuffer) + "</p>";
    page += "</body>";
    page += "</html>";
  }
  update_page = false;
}

// Setup function
void setup() 
{
  // Initialize serial communication for debugging
  Serial.begin(115200);

  // Initialize the display
  pinMode(SCREEN_SCREEN_ON_OFF, OUTPUT);
  digitalWrite(SCREEN_SCREEN_ON_OFF,HIGH); // turn ON the screen (open the gate in mosfet)
  screen.begin();
  screen.setRotation(3); // Adjust rotation as needed for your display orientation
  screen.fillScreen(ILI9341_WHITE);  // Fill the screen with a color
  screen.setCursor(10, 100);
  screen.setTextColor(ILI9341_BLACK);
  screen.setTextSize(2);
  screen.print("Welcome...");

  // Initialize the switch pin
  pinMode(ESP_SWITCH, INPUT);
  
  // Initialize serial communication with the GM810 barcode scanner
  pinMode(BARCODE_ON_OFF, OUTPUT);
  digitalWrite(BARCODE_ON_OFF,HIGH); // turn ON the barcode (enable the dc to dc 5V)
  barcode.begin(9600, SERIAL_8N1, ESP_RX_BARCODE_TX, ESP_TX_BARCODE_RX); // RX pin 16, TX pin 17

  // Initialize RTC 
  Wire.begin(RTC_SDA, RTC_SCL); // SDA, SCL for ESP32
  if (!RTC.begin()) 
  {
    Serial.println("Couldn't find RTC");
    while (1); // If RTC is not found, stop the program
  }
  RTC.adjust(DateTime(2024, 11, 14, 16, 16, 00)); // Set the date to 2024-11-05 12:30:00
  delay(5000);

  // Start the ESP32 as an Access Point
  WiFi.softAP(ssid, password);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){request->send(200, "text/html", page);}); // Serve a simple web page
  server.begin(); // Start the server
  
  // Initialize the blue LED pin
  pinMode(LED_PIN, OUTPUT);

  // Initialize the battry feedback pin
  pinMode(BAT_FEEDBACK_PIN, INPUT);

  // Get battry percentage
  float voltage = ((analogRead(25) * (3.3 / 4095.0))/3.3)*100;
  int battry_percentage = voltage;

  // start string at the screen with wifi ip and battry percentage
  screen.fillScreen(ILI9341_WHITE);
  screen.setTextColor(ILI9341_BLACK);
  screen.setCursor(10, 20);
  screen.print("Battry " + String(battry_percentage) + "%" );
  screen.setCursor(10, 80);
  screen.print("Press to start");
  screen.setCursor(10, 220);
  screen.print("WIFI IP:" + WiFi.softAPIP().toString());
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
        delay(20);
      }
      return barcodeData;
    }
    if (digitalRead(ESP_SWITCH) == LOW && !golden_mode)
    {
      barcodeData = "";
      return barcodeData;
    }
  }
  return barcodeData;
}

void loop() 
{
  if (digitalRead(ESP_SWITCH) == LOW)
  {
    // Get battry percentage 
    float voltage = ((analogRead(25) * (3.3 / 4095.0))/3.3)*100;
    int battry_percentage = voltage;

    golden_mode = true;
    screen.fillScreen(ILI9341_WHITE);
    screen.setTextColor(ILI9341_BLACK);
    screen.setCursor(10, 20);
    screen.print("Battry " + String(battry_percentage) + "%" );
    screen.setCursor(10, 100);
    screen.print("Scan golden Barcode");
    screen.setCursor(10, 220);
    screen.print("WIFI IP:" + WiFi.softAPIP().toString());
    // Read the golden barcode
    golden_barcode = readBarcode();
    screen.fillScreen(ILI9341_WHITE);  // Clear the screen
    screen.setCursor(10, 60);
    screen.setTextColor(ILI9341_BLACK);
    screen.print("Golden Barcode: ");
    screen.setCursor(10, 80);
    screen.print(golden_barcode);
    screen.setCursor(10, 120);
    screen.print("Scan target Barcode");
    screen.setCursor(10, 140);
    screen.print("Or");
    screen.setCursor(10, 160);
    screen.print("Press to scan new golden");
    golden_mode = false;

    // Read target barcodes until switch press
    while(1)
    {
      // Read the target barcode 
      target_barcode = readBarcode();

      if (target_barcode == "")
      {
        return;
      }
      // Format the date and time into a single string
      char datetimeBuffer[30];
      snprintf(datetimeBuffer, sizeof(datetimeBuffer), "%02d/%02d/%04d %02d:%02d:%02d", RTC.now().day(), RTC.now().month(), RTC.now().year(), RTC.now().hour(), RTC.now().minute(), RTC.now().second());
      // Compare the barcodes and display results
      if (golden_barcode == target_barcode)
      {
        screen.fillScreen(ILI9341_GREEN);  // Fill the screen with a color
        screen.setTextColor(ILI9341_BLACK);
        screen.setCursor(10, 20);
        screen.print(datetimeBuffer);
        screen.setCursor(10, 40);
        screen.print("Identical Barcodes");
        digitalWrite(LED_PIN,HIGH); // turn ON the blue LED 
        delay(500);
        digitalWrite(LED_PIN,LOW); // turn OFF the blue LED
      }
      else 
      {
        screen.fillScreen(ILI9341_RED);  // Fill the screen with a color
        screen.setTextColor(ILI9341_BLACK);
        screen.setCursor(10, 20);
        screen.print(datetimeBuffer);
        screen.setCursor(10, 40);
        screen.print("Different Barcodes");
        delay(500);
      }
      screen.setCursor(10, 60);
      screen.print("Golden Barcode: ");
      screen.setCursor(10, 80);
      screen.print(golden_barcode);
      screen.setCursor(10, 100);
      screen.print("Target Barcode: ");
      screen.setCursor(10, 120);
      screen.println(target_barcode);
      screen.setCursor(10, 160);
      screen.print("Scan next target Barcode");
      screen.setCursor(10, 180);
      screen.print("Or");
      screen.setCursor(10, 200);
      screen.print("Press to scan new golden");
      update_page = true;
      refresh_page();
    }
  }
}
