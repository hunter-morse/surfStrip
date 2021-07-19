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
    startIndex = startIndex + 1; /* motion speed */
    
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
