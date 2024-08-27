
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

// The credentials for you WLAN

const char* ssid = "YourSSID";
const char* password = "YourPassword";



int led_pin = LED_BUILTIN;
unsigned long led_blink_interval = 5000;

unsigned long led_pulse_duration = 500;   // light the led for n milliseconds
unsigned long led_last_enabled = 0;        // timepoint in millis when the led was last turned on

WebServer server(80);

void setup() {
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);

  Serial.begin(9600);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", onHttpRequest);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() 
{
    // Handle incoming HTTP requests
    server.handleClient();

    // Toggle the LED
    unsigned long now = millis();
    if( now > (led_last_enabled + led_pulse_duration) )
    {
        digitalWrite(led_pin, LOW);
    }
}



void onHttpRequest() 
{
  // Check the 'event' parameter

  String event = server.arg("event");
  if(event == "BLINK")
  {
      digitalWrite(led_pin, HIGH);
      led_last_enabled = millis();


      // To get the current blink rate:
      //
      // float blinkrate = server.arg("bpm").toFloat();
  }


  String message = "<html><body><h1>Received Parameters</h1>";
  if (server.args() > 0) 
  {
    for (int i = 0; i < server.args(); i++) 
    {
      message += "<p>" + server.argName(i) + ": " + server.arg(i) + "</p>";
    }

  } else {
    message += "<p>No parameters received</p>";
  }
  
  message += "</body></html>";
  server.send(200, "text/html", message);
}