#include <SPI.h>
#include <FastLED.h>
#include <WiFiNINA.h>
#include <ArduinoJson.h>
#include <ArduinoHttpClient.h>
#include "Firebase_Arduino_WiFiNINA.h"

#include "wifi_creds.h"
#include "firebase_creds.h"

// WiFi Params
char ssid[] = WIFI_SSID;
char pass[] = WIFI_PASS;
int status = WL_IDLE_STATUS;

// Firebase Server 
FirebaseData firebaseData;
String root = "/";
String path = "";

//FirebaseData firebaseData;
//String curPath = "/path";               // Path to stream data
//String datPath = "/spot/pismo/spotID";  // Default path with data stream

// Server to connect to
char server[] = "services.surfline.com";
int port = 80;

String command = "GET ";
String leader = "/kbyg/spots/forecasts/wave?";
String spotID = "";
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
#define LED_NUM_FT 7

const int numLEDs = LED_PER_FT * LED_NUM_FT;
CRGB leds[numLEDs];
int litLEDs = 0;

// General
#define HOUR 3600000    // milliseconds in an hour


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
  Serial.println("Connected to wifi");

  // Connect to Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, WIFI_SSID, WIFI_PASS);
  Firebase.reconnectWiFi(true);

  // Check stream connection
  printFbStreamStatus(firebaseData, root);
  Serial.println("xxxxx");
  

  // Setup FastLED
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, numLEDs);

}

void loop() {
  // Update data path
  /*
  String newDatPath = readFbStreamString(firebaseDataPath); //
  if (newDatPath != datPath){
    datPath = newDatPath;
    firebaseData = updateDatPath(datPath);
  }
  */

  // Appropriately save data
  

  // Update spotID
  Serial.print("Grabbing spot ID");
  spotID = readFbStreamString(firebaseData);

  // Print spotID for debug
  Serial.print("Spot ID: ");
  Serial.println(spotID);
  
  // Make GET request for surf data
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
  Serial.println("Status Code: " + (String)statusCode);
  String cResponse = client.responseBody();

  // Parse JSON from server
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
  Serial.println(maxSurf);

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
    delay(HOUR);
  }
}


void printWiFiStatus() {
  // Print WiFi connection status
  // Input: n/a
  // Output: n/a
  IPAddress ip = WiFi.localIP();
  long rssi = WiFi.RSSI();
}


int printFbStreamStatus(FirebaseData fbData, String path){
  // Print error message if Firebase stream can't be established
  // Input: FirebaseData object and path to stream
  // Output: n/a
  if(!Firebase.beginStream(fbData, path)){
    Serial.println("------Can't begin stream connection------");
    Serial.println("REASON: " + fbData.errorReason());
    Serial.println();
    return 1;
  } else {
    Serial.println("Connected to database");
    return 0;
  }
}


String readFbStreamString(FirebaseData fbData){
  // Check if able to read fbData stream. If so read; else print error
  // Input: FirebaseData object, fbData
  // Output: String read from stream
  String retStr = "";
  
  if (!Firebase.readStream(firebaseData))
  {
    Serial.println("Can't read stream data");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println();
  }

  if (firebaseData.streamTimeout())
  {
    Serial.println("Stream timeout, resume streaming...");
    Serial.println();
  }
  if (firebaseData.streamAvailable())
  {
    Serial.println("-------Stream Data available-------");
    Serial.print("VALUE: ");
    if (firebaseData.dataType() != "string")
      Serial.println("Not a String");
    else if (firebaseData.dataType() == "string")
      Serial.println(firebaseData.stringData());
    Serial.println();
  }
  else 
  {
    Serial.println("-------Stream Data not available-------");

  }
  return retStr;
}

FirebaseData updateDatPath(String newDatPath){
  // Update the FirebaseData object with new datapath
  // Input: data path string 
  // Return: FirebaseData object associated with new path
  FirebaseData newFbD;
  if(printFbStreamStatus(newFbD, newDatPath)){
    Serial.println("Error updating DatPath");
  }
  return newFbD;
}


void updateSpot(String){
  if (firebaseData.streamAvailable()){
    if (firebaseData.dataPath() != "/path"){
      return;
      //return 1;
    } else {
      path = firebaseData.stringData();
    }
  }
}


void blinkOnBoard(int del){
  // Purpose: Blink onboard LED
  // Input: Delay in ms
  // Output: N/A
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
