#include <M5Core2.h>
#include <WiFi.h>
#include <WebServer.h>

// Wi-Fi network created by the M5Core2
const char* ssid = "M5Core2_AP";
const char* password = "12345678";

// Create web server on port 80
WebServer server(80);

// Current screen color
uint16_t currentColor = BLACK;

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>M5Core2 Color Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      font-family: Arial;
      text-align: center;
      background: #222;
      color: white;
      padding-top: 40px;
    }

    button {
      width: 200px;
      height: 70px;
      margin: 10px;
      font-size: 22px;
      border: none;
      border-radius: 12px;
      cursor: pointer;
    }

    .red {
      background: red;
      color: white;
    }

    .blue {
      background: blue;
      color: white;
    }
  </style>
</head>

<body>
  <h1>M5Core2 Color Control</h1>

  <button class="red" onclick="location.href='/red'">
    RED
  </button>

  <button class="blue" onclick="location.href='/blue'">
    BLUE
  </button>

</body>
</html>
)rawliteral";

  server.send(200, "text/html", html);
}

void handleRed() {
  currentColor = RED;
  M5.Lcd.fillScreen(RED);

  server.sendHeader("Location", "/");
  server.send(303);
}

void handleBlue() {
  currentColor = BLUE;
  M5.Lcd.fillScreen(BLUE);

  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  M5.begin();

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(20, 20);
  M5.Lcd.println("Starting Soft AP...");

  // Create Wi-Fi access point
  WiFi.softAP(ssid, password);

  // Display network information
  IPAddress IP = WiFi.softAPIP();

  M5.Lcd.setCursor(20, 60);
  M5.Lcd.println("WiFi:");
  M5.Lcd.println(ssid);

  M5.Lcd.setCursor(20, 110);
  M5.Lcd.println("IP Address:");
  M5.Lcd.println(IP);

  // Webpage
  server.on("/", handleRoot);

  // Buttons
  server.on("/red", handleRed);
  server.on("/blue", handleBlue);

  // Start server
  server.begin();

  Serial.begin(115200);
  Serial.println("Soft AP started");
  Serial.print("IP address: ");
  Serial.println(IP);
}

void loop() {
  // Check for incoming webpage requests
  server.handleClient();
}