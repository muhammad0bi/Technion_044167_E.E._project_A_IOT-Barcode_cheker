#include <HardwareSerial.h>

// Create an instance of the HardwareSerial class for the ESP32
HardwareSerial mySerial(1);


void setup() {
  // Initialize serial communication with the Serial Monitor
  Serial.begin(115200);
  // power the barcode
  pinMode(22,OUTPUT);
  digitalWrite(22,HIGH);
  // Initialize serial communication with the GM810 barcode scanner through TTL-232
  // Ensure the baud rate matches the GM810 scanner's setting
  mySerial.begin(9600, SERIAL_8N1, 16, 17); // RX pin 16, TX pin 17
  
  Serial.println("Barcode Scanner Ready");
}

void loop() {
  // Check if there is data available from the scanner
  //Serial.print("Scanned Barcode: \n");
  if (mySerial.available()) {
    String barcode = "";
    // Read the incoming data
    while (mySerial.available()) 
    {
      char c = mySerial.read();
      barcode += c;
      Serial.print("i am reading \n");
      delay(20);
    }
    Serial.print("Scanned Barcode: ");
    Serial.println(barcode);
  }
}
