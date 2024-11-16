#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// Define the pins for the ESP32
#define TFT_CS    32
#define TFT_RST   4
#define TFT_DC    5
#define TFT_MOSI  23
#define TFT_CLK   18

// Create an instance of the ILI9341 display
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// Setup function
void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);
  Serial.println("Initializing...");
  // power the screen
  pinMode(33,OUTPUT);
  digitalWrite(33,HIGH);
  // Initialize the display
  tft.begin();

  // Set the rotation of the display
  tft.setRotation(3); // Adjust rotation as needed for your display orientation
  
  // Fill the screen with a color (e.g., black)
  tft.fillScreen(ILI9341_BLACK);

  // Draw a few test shapes
  tft.fillRect(10, 10, 100, 50, ILI9341_RED);
  delay(5000);
  tft.drawLine(0, 0, tft.width(), tft.height(), ILI9341_GREEN);
  delay(5000);
  tft.drawCircle(tft.width()/2, tft.height()/2, 50, ILI9341_BLUE);
  delay(5000);

  // Print a test message
  tft.setCursor(10, 100);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print("Hello, World!");
}

// Loop function (runs repeatedly)
void loop() {
  // Nothing to do here
}
