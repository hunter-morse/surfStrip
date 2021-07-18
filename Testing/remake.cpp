#include "Firebase_Arduino_WiFiNINA.h"
#include "wifi_creds.h"
#include "firebase_creds.h"

#define SUCCESS 0
#define FAILURE 1


void printWiFiStatus(void) {
	// Print WiFi connection status 
	// {n/a; n/a}
	IPAddress ip = WiFi.localIP();
	long rssi = WiFi.RSSI();			// recieved signal strength in dBm
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
		connection_status = WiFi.begin(ssid, pass)
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
		Serial.println("REASON: " + firebaseData.errorReason());
		Serial.println("--- ---");
	}

	if(fb.streamTimeout()){
		Serial.println("Stream timeout, resume streaming...\n");
	}

	if(fb.streamAvailable()){
		Serial.println("--- Stream data available ---");
		Serial.println("STREAM PATH: " + firebaseData.streamPath());
   		Serial.println("EVENT PATH: " + firebaseData.dataPath());
    	Serial.println("DATA TYPE: " + firebaseData.dataType());
    	Serial.println("EVENT TYPE: " + firebaseData.eventType());
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



void mainloop(FirebaseData fb){
	String dataString = "";

	// Connect to database
	connectToDatabase();
	if(printStreamStatus(fb)){
		dataString = fb.stringData();
		Serial.println("~mainloop: dataString- " + dataString);
	}
}




void main(void) {
	// Set-up firebase client
	FirebaseData firebaseData;	// Database object
	char host[] = FIREBASE_HOST;
	char auth[] = FIREBASE_AUTH;
	char path[] = "/path"; 		// Path to path selector
	char *dataPath;				// Path to data


	// Set-up wifi and http clients
	char server[] = "services.surfline.com"
	int port = 80;
	char leader[] = "/kbyg/spots/forecasts/wave?";
	char *spotID;
	char time[]   = "&days=1&intervalHours=24";
	char correction[] = "&maxHeights=true";

	int wifi_status = WL_IDLE_STATUS;
	char ssid[] = WIFI_SSID
	char pass[] = WIFI_PASS

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
	if(!Firebase.beginStream(FirebaseData, path)){
		Serial.println("--- Can't begin stream connection ---");
		Serial.println("Reason: " + fb.errorReason());
		Serial.println("------")
	}


	// Begin main loop
	mainloop(firebaseData);


}
