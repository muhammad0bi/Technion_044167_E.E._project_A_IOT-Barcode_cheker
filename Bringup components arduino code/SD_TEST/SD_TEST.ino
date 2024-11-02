#include <SPI.h>
#include <SD.h>

const int chipSelect = 11; // Change to your CS pin

void setup() {
  Serial.begin(115200);

  // Initialize the SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("Initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");

  // Example of reading a file
  File dataFile = SD.open("example.txt");
  if (dataFile) {
    while (dataFile.available()) {
      Serial.write(dataFile.read());
    }
    dataFile.close();
  } else {
    Serial.println("Error opening file");
  }
}

void loop() {
  // Nothing here
}
