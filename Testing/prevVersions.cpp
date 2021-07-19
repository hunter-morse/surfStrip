/*

#include <SPI.h>
#include <FastLED.h>
#include <ArduinoJson.h>
#include <ArduinoHttpClient.h>

#include "wifi_creds.h"
#include "firebase_creds.h"
#include "Firebase_Arduino_WiFiNINA.h"

// Server to connect to
char server[] = "services.surfline.com";
int port = 80;

String root = "/path";
String location = "";
String locationID = "";
String leader = "/kbyg/spots/forecasts/wave?";
String numDays = "&days=1";
String numHours = "&intervalHours=24";
String corrected = "&maxHeights=true";

// Set-up Clients
FirebaseData fb;        // Firebase client for get and put reqs
WiFiClient wifi;        // Wifi
HttpClient client = HttpClient(wifi, server, port); 

// Json doc
DynamicJsonDocument doc(2048);

// Time Tracker
#define HOUR_IN_MILLIS 3600000
unsigned long startMillis;
unsigned long currMillis;

// LEDs lit
#define LED_PIN 7
#define LED_PER_FT 9
#define LED_NUM_FT 7

const int totalLEDs = LED_PER_FT * LED_NUM_FT;
CRGB LEDs[totalLEDs];
int litLEDs = 0;
int R = 0;
int G = 0;
int B = 0;

int connectToWifi(int status){
  // Connect to wifi client. If unable to connect after 10 attempts
  // then wait 30 seconds before reattempting
  int attempts = 0;
  while (status != WL_CONNECTED and attempts < 10)
  {
    status = WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print(".");
    delay(300);
    attempts ++;
  }

  if(status == WL_CONNECTED){
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();
  
    return 0;
  } else {
    Serial.println("Unable to establish connection to wifi client");
    Serial.println("Reatempting connection in: ");
    for(int i = 30; i > 0; i --){
      Serial.println(i);
      delay(1000);
    }
    return 1;
  }
}

String getSiteResponse(String fullPath){
  // Get max wave height as float
  Serial.println("Begin request");
  client.beginRequest();
  Serial.println("get");
  Serial.println("-- request path: " + fullPath);
  client.get(fullPath);
  Serial.println("end");
  client.endRequest();

  Serial.println("get status code");
  int statusCode = client.responseStatusCode();
  Serial.println("Status Code: " + (String)statusCode);
  String cResponse = client.responseBody();
  delay(300);
  Serial.println("response recieved");

  Serial.print("Status: ");
  if(statusCode == 0 or statusCode == 200){
    Serial.println("SUCCESS");
  } else if (statusCode == -1){
    Serial.println("ERROR: Connection Failed");
  } else if (statusCode == -2){
    Serial.println("ERROR: API Error");
  } else if (statusCode == -3){
    Serial.println("ERROR: Connection Timed Out");
  } else if (statusCode == -4){
    Serial.println("ERROR: Invalid Response");
  } else {
    Serial.println("ERROR");
  }

  return cResponse;
}

float getWaveHeight(DynamicJsonDocument d, String json){
  // Get max surf height for the hour
  Serial.println("Get error");
  Serial.println("String length: " + (String)json.length());
  DeserializationError err = deserializeJson(d, json);

  // Check error
  Serial.println("Check error");
  if(err){
    Serial.println("ERROR: Deserialization Error");
    return 0.0;
  }

  // Collect Json Data
  Serial.println("Get maxSurf");
  const float maxSurf = d["data"]["wave"][0]["surf"]["max"];

  Serial.println("--- Max Surf ---");
  Serial.println(maxSurf);
  Serial.println("--- ---");

  return maxSurf;
}

int updatePath(String root, String curPath){
  // Check to see if the value pointed to by the register has changed
  String newPath;
  if(Firebase.getString(fb, root)){
    newPath = fb.stringData();
  } else {
    Serial.println("Error getting data");
    return -1;
  }

  if(newPath == curPath){
    return 0;
  } else if (newPath != curPath){
    Serial.println("--- Result ---");
    Serial.println("PATH: " + fb.dataPath());
    Serial.println("TYPE: " + fb.dataType());
    Serial.println("VALUE: " + fb.stringData());
    location = newPath;
    Serial.println("--- ---");
    return 1;
  }
}

int updateLocationID(String loc){
  // Grab the spotID for the current location
  if(Firebase.getString(fb, loc + "/spotID")){
    Serial.println("--- Result ---");
    Serial.println("PATH: " + fb.dataPath());
    Serial.println("TYPE: " + fb.dataType());
    Serial.println("VALUE: " + fb.stringData());
    locationID = fb.stringData();
    Serial.println("--- ---");
    return 0;
  } else {
    Serial.println("Error getting data");
    return -1;
  }
}

void updateRGBValues(String loc){
  // Update RGB values based on location specified
  if(Firebase.getInt(fb, loc + "/R")){
    R = fb.intData();
    Serial.println("R: " + (String)R);
  }

  if(Firebase.getInt(fb, loc + "/G")){
    G = fb.intData();
    Serial.println("G: " + (String)G);
  }

  if(Firebase.getInt(fb, loc + "/B")){
    B = fb.intData();
    Serial.println("B: " + (String)B);
  }
}

int calculateNumLEDs(float waveHeight, int scale){
  int numLEDs = round(waveHeight * scale);
  Serial.println("Num LEDs: " + (String)numLEDs);
  return numLEDs;
}

void updateLEDs(int currLitLEDs, int newLitLEDs){
  if(currLitLEDs < newLitLEDs){
    // Turn on new LEDs
    //for(int currLED = currLitLEDs; currLED < newLitLEDs; currLED ++){
    for(int currLED = 0; currLED < newLitLEDs; currLED ++){
      LEDs[currLED] = CRGB(R, G, B);
      FastLED.show();
      delay(500);
    }
  } else if (currLitLEDs > newLitLEDs){
    // Turn off extra LEDs
    for(int currLED = currLitLEDs; currLED > newLitLEDs; currLED --){
      LEDs[currLED] = CRGB(0, 0, 0);
      FastLED.show();
      delay(500);
    }
  } else {
    return;
  }
}

void setup() {
  // Establish Serial
  Serial.begin(9600);
  delay(100);

  // Set-up debug LED
  pinMode(LED_BUILTIN, OUTPUT);

  // Set-up FastLED
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(LEDs, totalLEDs);

  // Connect to wifi
  Serial.print("Connecting to Wi-Fi");
  int status = WL_IDLE_STATUS;
  while(connectToWifi(status));

  // Connect to server
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, WIFI_SSID, WIFI_PASS);
  Firebase.reconnectWiFi(true);

  // Pull value from path
  updatePath(root, location); 
  updateLocationID(location);
  updateRGBValues(location);

  String fullPath = leader + locationID + numDays + numHours + corrected;

  String cResponse = getSiteResponse(fullPath);

  // Parse JSON from server
  float waveHeight = getWaveHeight(doc, cResponse);
  int numLEDs = calculateNumLEDs(waveHeight, LED_PER_FT);
  updateLEDs(litLEDs, numLEDs);

  // Start Timer 
  startMillis = millis();
}

void loop() {
  // update based on user input/time
  // update path -- if change then reset time and recalculate LEDs to light

  currMillis = millis();


  if (updatePath(root, location) or (unsigned long)(currMillis - startMillis) >= HOUR_IN_MILLIS){
    updateLocationID(location);
    updateRGBValues(location);

    String fullPath = leader + locationID + numDays + numHours + corrected;
    String siteResponse = getSiteResponse(fullPath);
    Serial.println("Recieved site response");

    delay(300);
    doc.clear();
    float waveHeight = getWaveHeight(doc, siteResponse);
    Serial.println("Calculated wave height");

    delay(300);
    int numLEDs = calculateNumLEDs(waveHeight, LED_PER_FT);
    Serial.println("Calculated new LED height");

    delay(300);
    updateLEDs(litLEDs, numLEDs);
    Serial.println("Updated LEDs");
    delay(300);

    startMillis = millis();
    Serial.println("Timer Reset: startMillis- "+ (String)startMillis + "; currMillis- " + (String)currMillis);
  }
}

*/

/*
#include <SPI.h>
#include <FastLED.h>
#include <ArduinoJson.h>
#include <ArduinoHttpClient.h>

#include "wifi_creds.h"
#include "firebase_creds.h"
#include "Firebase_Arduino_WiFiNINA.h"

// Server to connect to
char server[] = "services.surfline.com";
int port = 80;

String root = "/path";
String location = "";
String locationID = "";
String leader = "/kbyg/spots/forecasts/wave?";
String numDays = "&days=1";
String numHours = "&intervalHours=24";
String corrected = "&maxHeights=true";

// Set-up Clients
FirebaseData fb;        // Firebase client for get and put reqs
WiFiClient wifi;        // Wifi
HttpClient client = HttpClient(wifi, server, port); 

// Json doc
DynamicJsonDocument doc(8191);

// Time Tracker
#define HOUR_IN_MILLIS 3600000
unsigned long startMillis;
unsigned long currMillis;

// LEDs lit
#define LED_PIN 7
#define LED_PER_FT 9
#define LED_NUM_FT 7

const int totalLEDs = LED_PER_FT * LED_NUM_FT;
CRGB LEDs[totalLEDs];
int litLEDs = 0;


int connectToWifi(int status){
  // Connect to wifi client. If unable to connect after 10 attempts
  // then wait 30 seconds before reattempting
  int attempts = 0;
  while (status != WL_CONNECTED and attempts < 10)
  {
    status = WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print(".");
    delay(300);
    attempts ++;
  }

  if(status == WL_CONNECTED){
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();
  
    return 0;
  } else {
    Serial.println("Unable to establish connection to wifi client");
    Serial.println("Reatempting connection in: ");
    for(int i = 30; i > 0; i --){
      Serial.println(i);
      delay(1000);
    }
    return 1;
  }
}

String getSiteResponse(String fullPath){
  // Get max wave height as float
  Serial.println("Begin request");
  client.beginRequest();
  Serial.println("get");
  Serial.println("-- request path: " + fullPath);
  client.get(fullPath);
  Serial.println("end");
  client.endRequest();

  Serial.println("get status code");
  int statusCode = client.responseStatusCode();
  Serial.println("Status Code: " + (String)statusCode);
  String cResponse = client.responseBody();
  Serial.println("response recieved");

  Serial.print("Status: ");
  if(statusCode == 0 or statusCode == 200){
    Serial.println("SUCCESS");
  } else if (statusCode == -1){
    Serial.println("ERROR: Connection Failed");
  } else if (statusCode == -2){
    Serial.println("ERROR: API Error");
  } else if (statusCode == -3){
    Serial.println("ERROR: Connection Timed Out");
  } else if (statusCode == -4){
    Serial.println("ERROR: Invalid Response");
  } else {
    Serial.println("ERROR");
  }

  return cResponse;
}

float getWaveHeight(DynamicJsonDocument doc, String json){
  // Get max surf height for the hour
  DeserializationError err = deserializeJson(doc, json);

  // Check error
  if(err){
    Serial.println("ERROR: Deserialization Error");
    return 0.0;
  }

  // Collect Json Data
  const float maxSurf = doc["data"]["wave"][0]["surf"]["max"];

  Serial.println("--- Max Surf ---");
  Serial.println(maxSurf);
  Serial.println("--- ---");

  return maxSurf;
}

int updatePath(String root, String curPath){
  // Check to see if the value pointed to by the register has changed
  String newPath;
  if(Firebase.getString(fb, root)){
    newPath = fb.stringData();
  } else {
    Serial.println("Error getting data");
    return -1;
  }

  if(newPath == curPath){
    return 0;
  } else if (newPath != curPath){
    Serial.println("--- Result ---");
    Serial.println("PATH: " + fb.dataPath());
    Serial.println("TYPE: " + fb.dataType());
    Serial.println("VALUE: " + fb.stringData());
    location = newPath;
    Serial.println("--- ---");
    return 1;
  }
}

int updateLocationID(String loc){
  // Grab the spotID for the current location
  if(Firebase.getString(fb, loc + "/spotID")){
    Serial.println("--- Result ---");
    Serial.println("PATH: " + fb.dataPath());
    Serial.println("TYPE: " + fb.dataType());
    Serial.println("VALUE: " + fb.stringData());
    locationID = fb.stringData();
    Serial.println("--- ---");
    return 0;
  } else {
    Serial.println("Error getting data");
    return -1;
  }
}

int calculateNumLEDs(float waveHeight, int scale){
  int numLEDs = round(waveHeight * scale);
  Serial.println("Num LEDs: " + (String)numLEDs);
  return numLEDs;
}

void updateLEDs(int currLitLEDs, int newLitLEDs){
  if(currLitLEDs < newLitLEDs){
    // Turn on new LEDs
    for(int currLED = currLitLEDs; currLED < newLitLEDs; currLED ++){
      LEDs[currLED] = CRGB(0, 175, 255);
      FastLED.show();
      delay(500);
    }
  } else if (currLitLEDs > newLitLEDs){
    // Turn off extra LEDs
    for(int currLED = currLitLEDs; currLED > newLitLEDs; currLED --){
      LEDs[currLED] = CRGB(0, 0, 0);
      FastLED.show();
      delay(500);
    }
  } else {
    return;
  }
}

void setup() {
  // Establish Serial
  Serial.begin(9600);
  delay(100);

  // Set-up debug LED
  pinMode(LED_BUILTIN, OUTPUT);

  // Set-up FastLED
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(LEDs, totalLEDs);

  // Connect to wifi
  Serial.print("Connecting to Wi-Fi");
  int status = WL_IDLE_STATUS;
  while(connectToWifi(status));

  // Connect to server
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, WIFI_SSID, WIFI_PASS);
  Firebase.reconnectWiFi(true);

  // Pull value from path
  updatePath(root, location); 
  updateLocationID(location);

  String fullPath = leader + locationID + numDays + numHours + corrected;

  String cResponse = getSiteResponse(fullPath);

  // Parse JSON from server
  float waveHeight = getWaveHeight(doc, cResponse);

  // Start Timer 
  startMillis = millis();
}

void loop() {
  // update based on user input/time
  // update path -- if change then reset time and recalculate LEDs to light

  currMillis = millis();


  if (updatePath(root, location) or currMillis - startMillis >= HOUR_IN_MILLIS){
    updateLocationID(location);

    String fullPath = leader + locationID + numDays + numHours + corrected;
    String siteResponse = getSiteResponse(fullPath);

    float waveHeight = getWaveHeight(doc, siteResponse);
    int numLEDs = calculateNumLEDs(waveHeight, LED_PER_FT);

    updateLEDs(litLEDs, numLEDs);

    startMillis = millis();
    Serial.println("Timer Reset: startMillis- "+ (String)startMillis + "; currMillis- " + (String)currMillis);
  }
}
*/


/*
#include <SPI.h>
#include <FastLED.h>
#include <ArduinoJson.h>
#include <ArduinoHttpClient.h>

#include "wifi_creds.h"
#include "firebase_creds.h"
#include "Firebase_Arduino_WiFiNINA.h"

// Server to connect to
char server[] = "services.surfline.com";
int port = 80;

String root = "/path";
String location = "";
String locationID = "";
String leader = "/kbyg/spots/forecasts/wave?";
String numDays = "&days=1";
String numHours = "&intervalHours=24";
String corrected = "&maxHeights=true";


// Set-up Clients
FirebaseData fb;        // Firebase client for get and put reqs
WiFiClient wifi;        // Wifi
HttpClient client = HttpClient(wifi, server, port); 

// Json doc
DynamicJsonDocument doc(8191);

// Time Tracker
#define HOUR_IN_MILLIS 3600000
unsigned long startMillis;
unsigned long currMillis;

// LEDs lit
#define LED_PIN 7
#define LED_PER_FT 9
#define LED_NUM_FT 7

const int totalLEDs = LED_PER_FT * LED_NUM_FT;
CRGB LEDs[totalLEDs];
int litLEDs = 0;


int connectToWifi(int status){
  // Connect to wifi client. If unable to connect after 10 attempts
  // then wait 30 seconds before reattempting
  int attempts = 0;
  while (status != WL_CONNECTED and attempts < 10)
  {
    status = WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print(".");
    delay(300);
    attempts ++;
  }

  if(status == WL_CONNECTED){
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();
  
    return 0;
  } else {
    Serial.println("Unable to establish connection to wifi client");
    Serial.println("Reatempting connection in: ");
    for(int i = 30; i > 0; i --){
      Serial.println(i);
      delay(1000);
    }
    return 1;
  }
}

String getSiteResponse(String fullPath){
  // Get max wave height as float
  Serial.println("Begin request");
  client.beginRequest();
  Serial.println("get");
  Serial.println("-- request path: " + fullPath);
  client.get(fullPath);
  Serial.println("end");
  client.endRequest();

  Serial.println("get status code");
  int statusCode = client.responseStatusCode();
  Serial.println("Status Code: " + (String)statusCode);
  String cResponse = client.responseBody();
  Serial.println("response recieved");

  Serial.print("Status: ");
  if(statusCode == 0){
    Serial.println("SUCCESS");
  } else if (statusCode == -1){
    Serial.println("ERROR: Connection Failed");
  } else if (statusCode == -2){
    Serial.println("ERROR: API Error");
  } else if (statusCode == -3){
    Serial.println("ERROR: Connection Timed Out");
  } else if (statusCode == -4){
    Serial.println("ERROR: Invalid Response");
  }

  return cResponse;
}

float getWaveHeight(DynamicJsonDocument doc, String json){
  // Get max surf height for the hour
  DeserializationError err = deserializeJson(doc, json);

  // Check error
  if(err){
    Serial.println("ERROR: Deserialization Error");
    return 0.0;
  }

  // Collect Json Data
  const float maxSurf = doc["data"]["wave"][0]["surf"]["max"];

  Serial.println("--- Max Surf ---");
  Serial.println(maxSurf);
  Serial.println("--- ---");

  return maxSurf;
}

int updatePath(String root, String curPath){
  // Check to see if the value pointed to by the register has changed
  String newPath;
  if(Firebase.getString(fb, root)){
    newPath = fb.stringData();
  } else {
    Serial.println("Error getting data");
    return -1;
  }

  if(newPath == curPath){
    return 0;
  } else if (newPath != curPath){
    Serial.println("--- Result ---");
    Serial.println("PATH: " + fb.dataPath());
    Serial.println("TYPE: " + fb.dataType());
    Serial.println("VALUE: " + fb.stringData());
    location = newPath;
    Serial.println("--- ---");
    return 1;
  }
}

int updateLocationID(String loc){
  // Grab the spotID for the current location
  if(Firebase.getString(fb, loc + "/spotID")){
    Serial.println("--- Result ---");
    Serial.println("PATH: " + fb.dataPath());
    Serial.println("TYPE: " + fb.dataType());
    Serial.println("VALUE: " + fb.stringData());
    locationID = fb.stringData();
    Serial.println("--- ---");
    return 0;
  } else {
    Serial.println("Error getting data");
    return -1;
  }
}

int calculateNumLEDs(float waveHeight, int scale){
  int numLEDs = round(waveHeight * scale);
  Serial.println("Num LEDs: " + (String)numLEDs);
  return numLEDs;
}

void updateLEDs(int currLitLEDs, int newLitLEDs){
  if(currLitLEDs < newLitLEDs){
    // Turn on new LEDs
    for(int currLED = currLitLEDs; currLED < newLitLEDs; currLED ++){
      LEDs[currLED] = CRGB(0, 175, 255);
      FastLED.show();
      delay(500);
    }
  } else if (currLitLEDs > newLitLEDs){
    // Turn off extra LEDs
    for(int currLED = currLitLEDs; currLED > newLitLEDs; currLED --){
      LEDs[currLED] = CRGB(0, 0, 0);
      FastLED.show();
      delay(500);
    }
  } else {
    return;
  }
}



void setup() {
  // Establish Serial
  Serial.begin(9600);
  delay(100);

  // Set-up debug LED
  pinMode(LED_BUILTIN, OUTPUT);

  // Set-up FastLED
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(LEDs, totalLEDs);

  // Connect to wifi
  Serial.print("Connecting to Wi-Fi");
  int status = WL_IDLE_STATUS;
  while(connectToWifi(status));

  // Connect to server
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, WIFI_SSID, WIFI_PASS);
  Firebase.reconnectWiFi(true);

  // Pull value from path
  updatePath(root, location); 
  updateLocationID(location);

  String fullPath = leader + locationID + numDays + numHours + corrected;

  String cResponse = getSiteResponse(fullPath);

  // Parse JSON from server
  float waveHeight = getWaveHeight(doc, cResponse);
}

void loop() {
  // update based on user input/time
  // update path -- if change then reset time and recalculate LEDs to light

  current
  
  if (updatePath(root, location)){
    updateLocationID(location);

    String fullPath = leader + locationID + numDays + numHours + corrected;
    String siteResponse = getSiteResponse(fullPath);

    float waveHeight = getWaveHeight(doc, siteResponse);
    int numLEDs = calculateNumLEDs(waveHeight, LED_PER_FT);

    updateLEDs(litLEDs, numLEDs);
  }
}
*/



/*
#include "wifi_creds.h"
#include "firebase_creds.h"
#include "Firebase_Arduino_WiFiNINA.h"
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

#define HOUR_IN_MILLIS 3600000

// Server to connect to
char server[] = "services.surfline.com";
int port = 80;

String root = "/path";
String location = "";
String locationID = "";
String leader = "/kbyg/spots/forecasts/wave?";
String numDays = "&days=1";
String numHours = "&intervalHours=24";
String corrected = "&maxHeights=true";


// Set-up Clients
FirebaseData fb;        // Firebase client for get and put reqs
//FirebaseData fbStream;  // Firebase client for streaming
WiFiClient wifi;        // Wifi
HttpClient client = HttpClient(wifi, server, port); 

// Json doc
DynamicJsonDocument doc(8191);


// Time Tracker
unsigned long startMillis;
unsigned long currMillis;

int connectToWifi(int status){
  // Connect to wifi client. If unable to connect after 10 attempts
  // then wait 30 seconds before reattempting
  int attempts = 0;
  while (status != WL_CONNECTED and attempts < 10)
  {
    status = WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print(".");
    delay(300);
    attempts ++;
  }

  if(status == WL_CONNECTED){
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();
  
    return 0;
  } else {
    Serial.println("Unable to establish connection to wifi client");
    Serial.println("Reatempting connection in: ");
    for(int i = 30; i > 0; i --){
      Serial.println(i);
      delay(1000);
    }
    return 1;
  }
}

String getSiteResponse(String fullPath){
  // Get max wave height as float
  Serial.println("Begin request");
  client.beginRequest();
  Serial.println("get");
  Serial.println("-- request path: " + fullPath);
  client.get(fullPath);
  Serial.println("end");
  client.endRequest();

  Serial.println("get status code");
  int statusCode = client.responseStatusCode();
  Serial.println("Status Code: " + (String)statusCode);
  String cResponse = client.responseBody();
  Serial.println("response recieved");

  Serial.print("Status: ");
  if(statusCode == 0){
    Serial.println("SUCCESS");
  } else if (statusCode == -1){
    Serial.println("ERROR: Connection Failed");
  } else if (statusCode == -2){
    Serial.println("ERROR: API Error");
  } else if (statusCode == -3){
    Serial.println("ERROR: Connection Timed Out");
  } else if (statusCode == -4){
    Serial.println("ERROR: Invalid Response");
  }

  return cResponse;
}

float getWaveHeight(DynamicJsonDocument doc, String json){
  // Get max surf height for the hour
  DeserializationError err = deserializeJson(doc, json);

  // Check error
  if(err){
    Serial.println("ERROR: Deserialization Error");
    return 0.0;
  }

  // Collect Json Data
  const float maxSurf = doc["data"]["wave"][0]["surf"]["max"];

  Serial.println("--- Max Surf ---");
  Serial.println(maxSurf);
  Serial.println("--- ---");

  return maxSurf;
}

int updatePath(String root, String curPath){
  // Check to see if the value pointed to by the register has changed
  String newPath;
  if(Firebase.getString(fb, root)){
    //Serial.println("--- Result ---");
    //Serial.println("PATH: " + fb.dataPath());
    //Serial.println("TYPE: " + fb.dataType());
    //Serial.println("VALUE: " + fb.stringData());
    newPath = fb.stringData();
    //Serial.println("--- ---");
  } else {
    Serial.println("Error getting data");
    return -1;
  }

  if(newPath == curPath){
    return 0;
  } else if (newPath != curPath){
    Serial.println("--- Result ---");
    Serial.println("PATH: " + fb.dataPath());
    Serial.println("TYPE: " + fb.dataType());
    Serial.println("VALUE: " + fb.stringData());
    location = newPath;
    Serial.println("--- ---");
    return 1;
  }
}

int updateLocationID(String loc){
  // Grab the spotID for the current location
  if(Firebase.getString(fb, loc + "/spotID")){
    Serial.println("--- Result ---");
    Serial.println("PATH: " + fb.dataPath());
    Serial.println("TYPE: " + fb.dataType());
    Serial.println("VALUE: " + fb.stringData());
    locationID = fb.stringData();
    Serial.println("--- ---");
    return 0;
  } else {
    Serial.println("Error getting data");
    return -1;
  }
}


void setup() {

  // Establish Serial
  Serial.begin(9600);
  delay(100);

  // Connect to wifi
  Serial.print("Connecting to Wi-Fi");
  int status = WL_IDLE_STATUS;
  while(connectToWifi(status));

  // Connect to server
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, WIFI_SSID, WIFI_PASS);
  Firebase.reconnectWiFi(true);

  // Pull value from path
  //String root = "/path";
  updatePath(root, location); 
  updateLocationID(location);
  String fullPath = leader + locationID + numDays + numHours + corrected;
  String cResponse = getSiteResponse(fullPath);

  // Parse JSON from server
  float waveHeight = getWaveHeight(doc, cResponse);
}

void loop() {
  // update based on user input/time
  // update path -- if change then reset time and 

  if (updatePath(root, location)){
    updateLocationID(location);

    String fullPath = leader + locationID + numDays + numHours + corrected;
    String siteResponse = getSiteResponse(fullPath);

    getWaveHeight(doc, siteResponse);
  }
}

*/





/*
#include "wifi_creds.h"
#include "firebase_creds.h"
#include "Firebase_Arduino_WiFiNINA.h"
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

#define HOUR_IN_MILLIS 3600000

// Server to connect to
char server[] = "services.surfline.com";
int port = 80;

String root = "/path";
String location = "";
String locationID = "";
String leader = "/kbyg/spots/forecasts/wave?";
String numDays = "&days=1";
String numHours = "&intervalHours=24";
String corrected = "&maxHeights=true";


// Set-up Clients
FirebaseData fb;        // Firebase client for get and put reqs
//FirebaseData fbStream;  // Firebase client for streaming
WiFiClient wifi;        // Wifi
HttpClient client = HttpClient(wifi, server, port); 


// Time Tracker
unsigned long startMillis;
unsigned long currMillis;

int connectToWifi(int status){
  // Connect to wifi client. If unable to connect after 10 attempts
  // then wait 30 seconds before reattempting
  int attempts = 0;
  while (status != WL_CONNECTED and attempts < 10)
  {
    status = WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print(".");
    delay(300);
    attempts ++;
  }

  if(status == WL_CONNECTED){
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();
  
    return 0;
  } else {
    Serial.println("Unable to establish connection to wifi client");
    Serial.println("Reatempting connection in: ");
    for(int i = 30; i > 0; i --){
      Serial.println(i);
      delay(1000);
    }
    return 1;
  }
}

String getSiteResponse(String fullPath){
  // Get max wave height as float
  Serial.println("Begin request");
  client.beginRequest();
  Serial.println("get");
  Serial.println("-- request path: " + fullPath);
  client.get(fullPath);
  Serial.println("end");
  client.endRequest();

  Serial.println("get status code");
  int statusCode = client.responseStatusCode();
  Serial.println("Status Code: " + (String)statusCode);
  String cResponse = client.responseBody();
  Serial.println("response recieved");

  Serial.print("Status: ");
  if(statusCode == 0){
    Serial.println("SUCCESS");
  } else if (statusCode == -1){
    Serial.println("ERROR: Connection Failed");
  } else if (statusCode == -2){
    Serial.println("ERROR: API Error");
  } else if (statusCode == -3){
    Serial.println("ERROR: Connection Timed Out");
  } else if (statusCode == -4){
    Serial.println("ERROR: Invalid Response");
  }

  return cResponse;
}

float getWaveHeight(DynamicJsonDocument doc, String json){
  // Get max surf height for the hour
  DeserializationError err = deserializeJson(doc, json);

  // Check error
  if(err){
    Serial.println("ERROR: Deserialization Error");
    return 0.0;
  }

  // Collect Json Data
  const float maxSurf = doc["data"]["wave"][0]["surf"]["max"];

  Serial.println("--- Max Surf ---");
  Serial.println(maxSurf);
  Serial.println("--- ---");

  return maxSurf;
}

int updatePath(String root, String curPath){
  // Check to see if the value pointed to by the register has changed
  String newPath;
  if(Firebase.getString(fb, root)){
    //Serial.println("--- Result ---");
    //Serial.println("PATH: " + fb.dataPath());
    //Serial.println("TYPE: " + fb.dataType());
    //Serial.println("VALUE: " + fb.stringData());
    newPath = fb.stringData();
    //Serial.println("--- ---");
  } else {
    Serial.println("Error getting data");
    return -1;
  }

  if(newPath == curPath){
    return 0;
  } else if (newPath != curPath){
    Serial.println("--- Result ---");
    Serial.println("PATH: " + fb.dataPath());
    Serial.println("TYPE: " + fb.dataType());
    Serial.println("VALUE: " + fb.stringData());
    location = newPath;
    Serial.println("--- ---");
    return 1;
  }
}

int updateLocationID(String loc){
  // Grab the spotID for the current location
  if(Firebase.getString(fb, loc + "/spotID")){
    Serial.println("--- Result ---");
    Serial.println("PATH: " + fb.dataPath());
    Serial.println("TYPE: " + fb.dataType());
    Serial.println("VALUE: " + fb.stringData());
    locationID = fb.stringData();
    Serial.println("--- ---");
    return 0;
  } else {
    Serial.println("Error getting data");
    return -1;
  }
}


void setup() {

  // Establish Serial
  Serial.begin(9600);
  delay(100);

  // Connect to wifi
  Serial.print("Connecting to Wi-Fi");
  int status = WL_IDLE_STATUS;
  while(connectToWifi(status));

  // Connect to server
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, WIFI_SSID, WIFI_PASS);
  Firebase.reconnectWiFi(true);

  // Pull value from path
  // String root = "/path";
  updatePath(root, location); 
  updateLocationID(location);

  String fullPath = leader + locationID + numDays + numHours + corrected;
  String cResponse = getSiteResponse(fullPath);

  // Parse JSON from server
  DynamicJsonDocument doc(8191);
  float waveHeight = getWaveHeight(doc, cResponse);
  
 

}

void loop() {
  // update based on user input/time
  // update path -- if change then reset time and 
  if (updatePath(root, location)){
    updateLocationID(location);
  }
  delay(5000);

}
*/


/*
#include "wifi_creds.h"
#include "firebase_creds.h"
#include "Firebase_Arduino_WiFiNINA.h"
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

#define HOUR_IN_MILLIS 3600000

// Server to connect to
char server[] = "services.surfline.com";
int port = 80;

String path;
String leader = "/kbyg/spots/forecasts/wave?";
String numDays = "&days=1";
String numHours = "&intervalHours=24";
String corrected = "&maxHeights=true";


// Set-up Clients
FirebaseData fb;        // Firebase
WiFiClient wifi;        // Wifi
HttpClient client = HttpClient(wifi, server, port); 


// Time Tracker
unsigned long startMillis;
unsigned long currMillis;

int connectToWifi(int status){
  // Connect to wifi client. If unable to connect after 10 attempts
  // then wait 30 seconds before reattempting
  int attempts = 0;
  while (status != WL_CONNECTED and attempts < 10)
  {
    status = WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print(".");
    delay(300);
    attempts ++;
  }

  if(status == WL_CONNECTED){
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();
  
    return 0;
  } else {
    Serial.println("Unable to establish connection to wifi client");
    Serial.println("Reatempting connection in: ");
    for(int i = 30; i > 0; i --){
      Serial.println(i);
      delay(1000);
    }
    return 1;
  }
}

String getSiteResponse(String fullPath){
  // Get max wave height as float
  Serial.println("Begin request");
  client.beginRequest();
  Serial.println("get");
  Serial.println("-- request path: " + fullPath);
  client.get(fullPath);
  Serial.println("end");
  client.endRequest();

  Serial.println("get status code");
  int statusCode = client.responseStatusCode();
  Serial.println("Status Code: " + (String)statusCode);
  String cResponse = client.responseBody();
  Serial.println("response recieved");

  Serial.print("Status: ");
  if(statusCode == 0){
    Serial.println("SUCCESS");
  } else if (statusCode == -1){
    Serial.println("ERROR: Connection Failed");
  } else if (statusCode == -2){
    Serial.println("ERROR: API Error");
  } else if (statusCode == -3){
    Serial.println("ERROR: Connection Timed Out");
  } else if (statusCode == -4){
    Serial.println("ERROR: Invalid Response");
  }

  return cResponse;
}

float getWaveHeight(DynamicJsonDocument doc, String json){
  // Get max surf height for the hour
  DeserializationError err = deserializeJson(doc, json);

  // Check error
  if(err){
    Serial.println("ERROR: Deserialization Error");
    return 0.0;
  }

  // Collect Json Data
  const float maxSurf = doc["data"]["wave"][0]["surf"]["max"];

  Serial.println("--- Max Surf ---");
  Serial.println(maxSurf);
  Serial.println("--- ---");

  return maxSurf;
}


void setup() {

  // Establish Serial
  Serial.begin(9600);
  delay(100);

  // Connect to wifi
  Serial.print("Connecting to Wi-Fi");
  int status = WL_IDLE_STATUS;
  while(connectToWifi(status));

  // Connect to server
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, WIFI_SSID, WIFI_PASS);
  Firebase.reconnectWiFi(true);

  // Http Client
  // WiFiSSLClient wifiClient = fb.getWiFiClient();
  // HttpClient client = HttpClient(wifiClient, server, port);

  // Pull value from path
  String root = "/path";
  
  if(Firebase.getString(fb, root)){
    Serial.println("--- Result ---");
    Serial.println("PATH: " + fb.dataPath());
    Serial.println("TYPE: " + fb.dataType());
    Serial.println("VALUE: " + fb.stringData());
    path = fb.stringData();
    Serial.println("--- ---");
  } else {
    Serial.println("Error getting data");
  }

  if(Firebase.getString(fb, path + "/spotID")){
    Serial.println("--- Result ---");
    Serial.println("PATH: " + fb.dataPath());
    Serial.println("TYPE: " + fb.dataType());
    Serial.println("VALUE: " + fb.stringData());
    path = fb.stringData();
    Serial.println("--- ---");
  } else {
    Serial.println("Error getting data");
  }

  String fullPath = leader + path + numDays + numHours + corrected;
  String cResponse = getSiteResponse(fullPath);

  // Parse JSON from server
  DynamicJsonDocument doc(8191);
  float waveHeight = getWaveHeight(doc, cResponse);
  

}

void loop() {
  // update based on user input/time

}


*/

/*
#include "wifi_creds.h"
#include "firebase_creds.h"
#include "Firebase_Arduino_WiFiNINA.h"
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

// Server to connect to
char server[] = "services.surfline.com";
int port = 80;

String path;
String leader = "/kbyg/spots/forecasts/wave?";
String numDays = "&days=1";
String numHours = "&intervalHours=24";
String corrected = "&maxHeights=true";


// Set-up Clients
FirebaseData fb;        // Firebase
WiFiClient wifi;        // Wifi
HttpClient client = HttpClient(wifi, server, port); 


int connectToWifi(int status){
  // Connect to wifi client. If unable to connect after 10 attempts
  // then wait 30 seconds before reattempting
  int attempts = 0;
  while (status != WL_CONNECTED and attempts < 10)
  {
    status = WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print(".");
    delay(300);
    attempts ++;
  }

  if(status == WL_CONNECTED){
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();
  
    return 0;
  } else {
    Serial.println("Unable to establish connection to wifi client");
    Serial.println("Reatempting connection in: ");
    for(int i = 30; i > 0; i --){
      Serial.println(i);
      delay(1000);
    }
    return 1;
  }
}



void setup() {

  // Establish Serial
  Serial.begin(9600);
  delay(100);

  // Connect to wifi
  Serial.print("Connecting to Wi-Fi");
  int status = WL_IDLE_STATUS;
  while(connectToWifi(status));

  *
  while (status != WL_CONNECTED)
  {
    status = WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  *

  // Connect to server
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, WIFI_SSID, WIFI_PASS);
  Firebase.reconnectWiFi(true);

  // Http Client
  // WiFiSSLClient wifiClient = fb.getWiFiClient();
  // HttpClient client = HttpClient(wifiClient, server, port);

  // Pull value from path
  String root = "/path";
  
  if(Firebase.getString(fb, root)){
    Serial.println("--- Result ---");
    Serial.println("PATH: " + fb.dataPath());
    Serial.println("TYPE: " + fb.dataType());
    Serial.println("VALUE: " + fb.stringData());
    path = fb.stringData();
    Serial.println("--- ---");
  } else {
    Serial.println("Error getting data");
  }

  if(Firebase.getString(fb, path + "/spotID")){
    Serial.println("--- Result ---");
    Serial.println("PATH: " + fb.dataPath());
    Serial.println("TYPE: " + fb.dataType());
    Serial.println("VALUE: " + fb.stringData());
    path = fb.stringData();
    Serial.println("--- ---");
  } else {
    Serial.println("Error getting data");
  }

  Serial.println("Begin request");
  client.beginRequest();
  Serial.println("get");
  String fullPath = leader + path + numDays + numHours + corrected;
  Serial.println("-- request path: " + fullPath);
  client.get(fullPath);
  Serial.println("end");
  client.endRequest();

  Serial.println("get status code");
  int statusCode = client.responseStatusCode();
  Serial.println("Status Code: " + (String)statusCode);
  String cResponse = client.responseBody();
  Serial.println("response recieved");

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

  Serial.println("--- Max Surf ---");
  Serial.println(maxSurf);
  Serial.println("--- ---");

  

}

void loop() {
  // put your main code here, to run repeatedly:
}
*/


/*
#include "wifi_creds.h"
#include "firebase_creds.h"
#include "Firebase_Arduino_WiFiNINA.h"
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

// Server to connect to
char server[] = "services.surfline.com";
int port = 80;

String path;
String leader = "/kbyg/spots/forecasts/wave?";
String numDays = "&days=1";
String numHours = "&intervalHours=24";
String corrected = "&maxHeights=true";


// Set-up Clients
FirebaseData fb;        // Firebase
WiFiClient wifi;        // Wifi
HttpClient client = HttpClient(wifi, server, port); 


void setup() {

  // Establish Serial
  Serial.begin(9600);
  delay(100);

  // Connect to wifi
  Serial.print("Connecting to Wi-Fi");
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED)
  {
    status = WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Connect to server
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, WIFI_SSID, WIFI_PASS);
  Firebase.reconnectWiFi(true);

  // Http Client
  // WiFiSSLClient wifiClient = fb.getWiFiClient();
  // HttpClient client = HttpClient(wifiClient, server, port);

  // Pull value from path
  String root = "/path";
  
  if(Firebase.getString(fb, root)){
    Serial.println("--- Result ---");
    Serial.println("PATH: " + fb.dataPath());
    Serial.println("TYPE: " + fb.dataType());
    Serial.println("VALUE: " + fb.stringData());
    path = fb.stringData();
    Serial.println("--- ---");
  } else {
    Serial.println("Error getting data");
  }

  if(Firebase.getString(fb, path + "/spotID")){
    Serial.println("--- Result ---");
    Serial.println("PATH: " + fb.dataPath());
    Serial.println("TYPE: " + fb.dataType());
    Serial.println("VALUE: " + fb.stringData());
    path = fb.stringData();
    Serial.println("--- ---");
  } else {
    Serial.println("Error getting data");
  }

  Serial.println("Begin request");
  client.beginRequest();
  Serial.println("get");
  String fullPath = leader + path + numDays + numHours + corrected;
  Serial.println("-- request path: " + fullPath);
  client.get(fullPath);
  Serial.println("end");
  client.endRequest();

  Serial.println("get status code");
  int statusCode = client.responseStatusCode();
  Serial.println("Status Code: " + (String)statusCode);
  String cResponse = client.responseBody();
  Serial.println("response recieved");

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

  Serial.println("--- Max Surf ---");
  Serial.println(maxSurf);
  Serial.println("--- ---");

  

}

void loop() {
  // put your main code here, to run repeatedly:
}

*/


/*
#include "wifi_creds.h"
#include "firebase_creds.h"
#include "Firebase_Arduino_WiFiNINA.h"
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>


FirebaseData fb;

// Server to connect to
char server[] = "services.surfline.com";
int port = 80;

String path;
String leader = "/kbyg/spots/forecasts/wave?";
String numDays = "&days=1";
String numHours = "&intervalHours=24";
String corrected = "&maxHeights=true";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(100);

  // Connect to wifi
  Serial.print("Connecting to Wi-Fi");
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED)
  {
    status = WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Connect to server
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, WIFI_SSID, WIFI_PASS);
  Firebase.reconnectWiFi(true);

  // Http Client
  WiFiSSLClient wifiClient = fb.getWiFiClient();
  HttpClient client = HttpClient(wifiClient, server, port);

  // Pull value from path
  String root = "/path";
  
  if(Firebase.getString(fb, root)){
    Serial.println("--- Result ---");
    Serial.println("PATH: " + fb.dataPath());
    Serial.println("TYPE: " + fb.dataType());
    Serial.println("VALUE: " + fb.stringData());
    path = fb.stringData();
    Serial.println("--- ---");
  } else {
    Serial.println("Error getting data");
  }

  if(Firebase.getString(fb, path + "/spotID")){
    Serial.println("--- Result ---");
    Serial.println("PATH: " + fb.dataPath());
    Serial.println("TYPE: " + fb.dataType());
    Serial.println("VALUE: " + fb.stringData());
    path = fb.stringData();
    Serial.println("--- ---");
  } else {
    Serial.println("Error getting data");
  }

  Serial.println("Begin request");
  client.beginRequest();
  Serial.println("get");
  client.get(leader + path + numDays + numHours + corrected);
  Serial.println("end");
  client.endRequest();

  Serial.println("get status code");
  int statusCode = client.responseStatusCode();
  Serial.println("Status Code: " + (String)statusCode);
  String cResponse = client.responseBody();
  Serial.println("response recieved");

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

  Serial.println("--- Max Surf ---");
  Serial.println(maxSurf);
  Serial.println("--- ---");

  

}

void loop() {
  // put your main code here, to run repeatedly:
  
  
  
}

*/