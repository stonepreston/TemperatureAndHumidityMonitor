#include <Arduino.h>
#include <SoftwareSerial.h>
#include <SparkFunESP8266WiFi.h>
#include "WifiConnector.h"

// Constructor
WifiConnector::WifiConnector(String name, String password) {

	networkName = name;
	networkPassword = password;

}

// esp8266.begin() verifies that the ESP8266 is operational
// and sets it up for the rest of the sketch.
// It returns either true or false -- indicating whether
// communication was successul or not.
// true
void WifiConnector::initializeESP8266() {

	int success = esp8266.begin();
	if (success != true) {

		Serial.println(F("Error talking to ESP8266."));
		errorLoop(success);

	}

	Serial.println(F("ESP8266 Shield Present"));
}

void WifiConnector::connectESP8266() {

    // The ESP8266 can be set to one of three modes:
    //  1 - ESP8266_MODE_STA - Station only
    //  2 - ESP8266_MODE_AP - Access point only
    //  3 - ESP8266_MODE_STAAP - Station/AP combo
    // Use esp8266.getMode() to check which mode it's in:
    int retVal = esp8266.getMode();
    if (retVal != ESP8266_MODE_STA) {

    	// If it's not in station mode.
	    // Use esp8266.setMode([mode]) to set it to a specified
	    // mode.
	    retVal = esp8266.setMode(ESP8266_MODE_STA);

		if (retVal < 0) {
			Serial.println(F("Error setting mode."));
			errorLoop(retVal);
		}
    }

    Serial.println(F("Mode set to station"));

    // esp8266.status() indicates the ESP8266's WiFi connect
    // status.
    // A return value of 1 indicates the device is already
    // connected. 0 indicates disconnected. (Negative values
    // equate to communication errors.)
    retVal = esp8266.status();

    if (retVal <= 0) {

		Serial.print(F("Connecting to "));
		Serial.println(networkName);
		// esp8266.connect([ssid], [psk]) connects the ESP8266
		// to a network.
		// On success the connect function returns a value >0
		// On fail, the function will either return:
		//  -1: TIMEOUT - The library has a set 30s timeout
		//  -3: FAIL - Couldn't connect to network.

		// the connect function accepts char arrays, so have to convert the strings
		char nameCharArray[networkName.length()];
		networkName.toCharArray(nameCharArray, networkName.length());
		char passCharArray[networkPassword.length()];
		networkPassword.toCharArray(passCharArray, networkPassword.length());
		retVal = esp8266.connect(nameCharArray, passCharArray);

		if (retVal < 0) {
		  Serial.println(F("Error connecting"));
		  errorLoop(retVal);
		}
    }

}

void WifiConnector::displayConnectionInfo() {


	char connectedSSID[24];
	memset(connectedSSID, 0, 24);
	// esp8266.getAP() can be used to check which AP the
	// ESP8266 is connected to. It returns an error code.
	// The connected AP is returned by reference as a parameter.
	int retVal = esp8266.getAP(connectedSSID);

	if (retVal > 0) {

		Serial.print(F("Connected to: "));
		Serial.println(connectedSSID);

	}

	// esp8266.localIP returns an IPAddress variable with the
	// ESP8266's current local IP address.
	IPAddress myIP = esp8266.localIP();
	Serial.print(F("My IP: ")); Serial.println(myIP);
}

void WifiConnector::errorLoop(int error) {

	Serial.print(F("Error: ")); Serial.println(error);
	Serial.println(F("Looping forever."));
	for (;;)
		;
}

