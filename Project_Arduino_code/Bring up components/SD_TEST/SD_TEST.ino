#include <SD_MMC.h>  // Use the SD_MMC library for SD cards via the SDMMC interface

#define SD_CLK   18  // SCLK (Clock)
#define SD_CMD   23  // CMD (Command)
#define SD_CS    26  // Chip Select
#define SD_DAT0  19  // DAT0 (Data)

void setup() {
  Serial.begin(115200);
  
  // Print an initial message to show the start of setup
  Serial.println("Starting SD card initialization...");
  delay(5000);  // Give time for the system to settle
  
  // Initialize the SD card with the correct mount point ("/sdcard")
  if (!SD_MMC.begin("/sdcard", SD_CS, SD_CLK, SD_CMD, SD_DAT0)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  
  Serial.println("SD card initialized successfully.");
}

void loop() {
  // Example: Write a file to the SD card
  File myFile = SD_MMC.open("/test.txt", FILE_WRITE);  // Open file for writing (overwrites existing)
  if (myFile) {
    myFile.println("Hello from ESP32 with 4-bit parallel mode!");
    myFile.close();  // Close the file
    Serial.println("Data written to file.");
  } else {
    Serial.println("Error opening file for writing.");
  }
  
  delay(1000); // Wait a second before looping again
}
