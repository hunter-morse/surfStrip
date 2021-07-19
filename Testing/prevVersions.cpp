/*




#include <FastLED.h>
#include <SPI.h>

#define LED_PIN     7
#define NUM_LEDS    14
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

// This example shows several ways to set up and use 'palettes' of colors
// with FastLED.
//
// These compact palettes provide an easy way to re-colorize your
// animation on the fly, quickly, easily, and with low overhead.
//
// USING palettes is MUCH simpler in practice than in theory, so first just
// run this sketch, and watch the pretty lights as you then read through
// the code.  Although this sketch has eight (or more) different color schemes,
// the entire sketch compiles down to about 6.5K on AVR.
//
// FastLED provides a few pre-configured color palettes, and makes it
// extremely easy to make up your own color schemes with palettes.
//
// Some notes on the more abstract 'theory and practice' of
// FastLED compact palettes are at the bottom of this file.



CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;


void setup() {
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
}


void loop()
{
    ChangePalettePeriodically();
    
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; * motion speed *
    
    FillLEDsFromPaletteColors( startIndex);
    
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}


// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
        if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
        if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
        if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
        if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
    }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};

*/


/*
#include "Firebase_Arduino_WiFiNINA.h"
#include "wifi_creds.h"
#include "firebase_creds.h"
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

#define SUCCESS 0
#define FAILURE 1


void printWiFiStatus(void) {
  // Print WiFi connection status 
  // {n/a; n/a}
  IPAddress ip = WiFi.localIP();
  long rssi = WiFi.RSSI();      // recieved signal strength in dBm
  Serial.print("RSSI: ");
  Serial.println(rssi);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

int connectToWifi(char *ssid, char *pass, int connection_status){
  // Connect to wifi. If 10+ failed attempts return error
  // {wifi ssid and password as char* + staus as int; [0/1] pass/fail }
  int attempts = 0;

  while(connection_status != WL_CONNECTED && attempts < 10){
    connection_status = WiFi.begin(ssid, pass);
    delay(1000);
    attempts ++;
  } 

  if(connection_status != WL_CONNECTED){
    Serial.println("ERROR: connectToWifi- Too many unsuccessful attemtps");
    return FAILURE;
  } else {
    Serial.println("Connected to wifi");
    return SUCCESS;
  }
}

void connectToDatabase(char *host, char *auth, char *ssid, char *pass){
  // Connect to database
  // {database and wifi creds; n/a}
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, WIFI_SSID, WIFI_PASS);
  Firebase.reconnectWiFi(true);
}


int printStreamStatus(FirebaseData fb, char *path){
  // Continuously attempt to read FB stream
  // {data and path; [0/1] 1 if string available, else 0}
  int strAvailable = 0;

  if(!Firebase.readStream(fb)){
    Serial.println("--- Can't read stream data ---");
    Serial.println("REASON: " + fb.errorReason());
    Serial.println("--- ---");
  }

  if(fb.streamTimeout()){
    Serial.println("Stream timeout, resume streaming...\n");
  }

  if(fb.streamAvailable()){
    Serial.println("--- Stream data available ---");
    Serial.println("STREAM PATH: " + fb.streamPath());
    Serial.println("EVENT PATH: " + fb.dataPath());
    Serial.println("DATA TYPE: " + fb.dataType());
    Serial.println("EVENT TYPE: " + fb.eventType());
    Serial.print("VALUE: ");
    if(fb.dataType() != "string"){
      Serial.println("Not a string");
    } else {
      Serial.println(fb.stringData());
      strAvailable = 1;
    }
    Serial.println();
  }

  return strAvailable;
}



void mainloop(FirebaseData fb, char *stream){
  String dataString = "";

  // Connect to database
  //connectToDatabase();
  if(printStreamStatus(fb, stream)){
    dataString = fb.stringData();
    Serial.println("~mainloop: dataString- " + dataString);
  }
}




int mainprog(void) {
  // Set-up firebase client
  FirebaseData firebaseData;  // Database object
  char host[] = FIREBASE_HOST;
  char auth[] = FIREBASE_AUTH;
  char path[] = "/path";      // Path to path selector
  char *dataPath;             // Path to data


  // Set-up wifi and http clients
  char server[] = "services.surfline.com";
  int port = 80;
  char leader[] = "/kbyg/spots/forecasts/wave?";
  char *spotID;
  char time[]   = "&days=1&intervalHours=24";
  char correction[] = "&maxHeights=true";

  int wifi_status = WL_IDLE_STATUS;
  char ssid[] = WIFI_SSID;
  char pass[] = WIFI_PASS;

  WiFiClient wifi;
  HttpClient client = HttpClient(wifi, server, port);

  // Connect to wifi
  if(connectToWifi(ssid, pass, wifi_status) == SUCCESS){
    printWiFiStatus();
    //blinkOnBoardLED(SUCCESS)
  } else {
    //blinkOnBoardLED(FAILURE)
    // WANT TO STAY IN FAILURE LOOP UNTIL RESET BUTTON IS PRESSED
    // ONCE PRESSED RESTART MAIN
  }

  // Connect to firebase
  connectToDatabase(host, auth, ssid, pass);
  if(!Firebase.beginStream(firebaseData, path)){
    Serial.println("--- Can't begin stream connection ---");
    Serial.println("Reason: " + firebaseData.errorReason());
    Serial.println("------");
  }


  // Begin main loop
  mainloop(firebaseData, path);

  return 0;
}

void setup(){
  
}

void loop(){
  mainprog();
  
}
*/

/*
#include "Firebase_Arduino_WiFiNINA.h"
#include "wifi_creds.h"
#include "firebase_creds.h"

//Define Firebase data object
FirebaseData firebaseData;

unsigned long sendDataPrevMillis = 0;

//String path = "/WiFiNiNa_Test/Stream";
String path = "/spot/morro";

uint16_t count = 0;

void setup()
{

  Serial.begin(115200);
  delay(100);
  Serial.println();

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
  
  //Provide the autntication data
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, WIFI_SSID, WIFI_PASS);
  Firebase.reconnectWiFi(true);

  if (!Firebase.beginStream(firebaseData, path))
  {
    Serial.println("------Can't begin stream connection------");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println();
  }
}

void loop()
{

  if (millis() - sendDataPrevMillis > 15000)
  {
    sendDataPrevMillis = millis();
    count++;
    
*
    if (Firebase.setString(firebaseData, path + "/String", "Hello World! " + String(count)))
    {
      Serial.println("----------Set result-----------");
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.println("TYPE: " + firebaseData.dataType());
      Serial.print("VALUE: ");
      if (firebaseData.dataType() == "int")
        Serial.println(firebaseData.intData());
      else if (firebaseData.dataType() == "float")
        Serial.println(firebaseData.floatData());
      else if (firebaseData.dataType() == "boolean")
        Serial.println(firebaseData.boolData() == 1 ? "true" : "false");
      else if (firebaseData.dataType() == "string")
        Serial.println(firebaseData.stringData());
      else if (firebaseData.dataType() == "json")
        Serial.println(firebaseData.jsonData());
      Serial.println("--------------------------------");
      Serial.println();
    }
    else
    {
      Serial.println("----------Can't set data--------");
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println("--------------------------------");
      Serial.println();
    }
*
  }


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
    Serial.println("STREAM PATH: " + firebaseData.streamPath());
    Serial.println("EVENT PATH: " + firebaseData.dataPath());
    Serial.println("DATA TYPE: " + firebaseData.dataType());
    Serial.println("EVENT TYPE: " + firebaseData.eventType());
    Serial.print("VALUE: ");
    if (firebaseData.dataType() == "int")
      Serial.println(firebaseData.intData());
    else if (firebaseData.dataType() == "float")
      Serial.println(firebaseData.floatData());
    else if (firebaseData.dataType() == "boolean")
      Serial.println(firebaseData.boolData() == 1 ? "true" : "false");
    else if (firebaseData.dataType() == "string")
      Serial.println(firebaseData.stringData());
    else if (firebaseData.dataType() == "json")
      Serial.println(firebaseData.jsonData());
    Serial.println();
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