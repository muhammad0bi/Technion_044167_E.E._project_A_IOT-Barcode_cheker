#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char *ssid = "ESP32-Access-Point";  // SSID for the AP
const char *password = "123456789";       // Password for the AP

AsyncWebServer server(80);  // Create a web server on port 80

void setup() {
  // Start serial communication
  Serial.begin(115200);

  // Start the ESP32 as an Access Point
  WiFi.softAP(ssid, password);

  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());  // Print the IP address of the AP

  // Serve a simple web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", "<h1>Welcome to ESP32 Web Server!</h1>");
  });

  // Start the server
  server.begin();
}

void loop() {
  // Nothing to do here
}
