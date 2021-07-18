#include <SPI.h>
#include <FastLED.h>
#include <WiFiNINA.h>
#include <ArduinoJson.h>
#include <ArduinoHttpClient.h>

#include "wifi_creds.h"

// WiFi Params
char ssid[] = WIFI_SSID;
char pass[] = WIFI_PASS;
int status = WL_IDLE_STATUS;

// Server to connect to
char server[] = "services.surfline.com";
int port = 80;

String command = "GET ";
String leader = "/kbyg/spots/forecasts/wave?";
String spotID = "spotId=584204214e65fad6a7709cfa";
String numDays = "&days=1";
String numHours = "&intervalHours=24";
String corrected = "&maxHeights=true";
String httpOpt = " HTTP/1.1";

// Wifi and HTTP Client
WiFiClient wifi;
HttpClient client = HttpClient(wifi, server, port);

// LED Params
#define LED_PIN 7
#define LED_PER_FT 9
#define LED_NUM_FT 8

const int numLEDs = LED_PER_FT * LED_NUM_FT;
CRGB leds[numLEDs];
int litLEDs = 0;

void setup() {
  // Debug Params
  pinMode(LED_BUILTIN, OUTPUT);

  // Connect to Wifi
  while(status != WL_CONNECTED) {
    status = WiFi.begin(ssid, pass);
    delay(1000);
  }

  // Connected to wifi
  printWiFiStatus();
  blinkOnBoard(50);
  blinkOnBoard(100);
  blinkOnBoard(50);

  // Setup FastLED
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, numLEDs);

}

void loop() {
  // Make GET request
  client.beginRequest();
  client.get(leader + spotID + numDays + numHours + corrected);
  client.endRequest();

  // Indicate ping
  leds[0] = CRGB(255, 0, 0);
  FastLED.show();
  delay(500);
  leds[0] = CRGB(0, 0, 0);
  FastLED.show();
  

  int statusCode = client.responseStatusCode();
  String cResponse = client.responseBody();

  // Parse JSON
  DynamicJsonDocument doc(8191);
  String json = cResponse;

  // Deserialization Error
  DeserializationError err = deserializeJson(doc, json);

  // Check error
  if(err){
    return;
  }

  // Collect Json Data
  const float maxSurf = doc["data"]["wave"][0]["surf"]["max"];

  // Light LEDs
  int toBeLitLEDs = round(maxSurf * LED_PER_FT);
  if((litLEDs - toBeLitLEDs) < 0){
    lightLEDs(litLEDs, toBeLitLEDs);
    litLEDs = toBeLitLEDs;
  } else if ((litLEDs - toBeLitLEDs) > 0){
    unLightLEDs(litLEDs, toBeLitLEDs);
    litLEDs = toBeLitLEDs;
  }
  
  
  // Case: server disconnects from wifi
  if(!wifi.connected()){
    wifi.stop();

    // Delay
    delay(3600000);
  }
}


void printWiFiStatus() {
  IPAddress ip = WiFi.localIP();
  long rssi = WiFi.RSSI();
}


// Blink on board LED
void blinkOnBoard(int del){
  digitalWrite(LED_BUILTIN, HIGH);
   delay(del);
   digitalWrite(LED_BUILTIN, LOW);
   delay(del);
}

// Light LED Strip
void lightLEDs(int first_LED, int last_LED) {

  // Indicate wave height with onboard LED
  for(int blinks = 0; blinks < last_LED/10; blinks ++){
    blinkOnBoard(500);
  }
  
  for(int n = 0; n < (last_LED - first_LED); n++){
    leds[n] = CRGB(0, 175, 255);
    FastLED.show();
  }
}

// Unlight LEDs
void unLightLEDs(int first_LED, int last_LED){
  for(int n = last_LED; n > first_LED; n --){
    leds[n] = CRGB(0, 0, 0);
    FastLED.show();
    delay(1500);
  }
}
