#include <SoftwareSerial.h>
#include <SparkFunESP8266WiFi.h>
#include <Arduino.h>
#include "config.h"
#include "DataPusher.h"


void DataPusher::push(float field1, float field2, float field3) {

	ESP8266Client client;
	int retVal = client.connect(destServer, 80);

	if (retVal <= 0) {

	    Serial.println(F("Failed to connect to server."));
	    return;

	}

	String postString = constructPostString(field1, field2, field3);

	printRequestToSerial(postString);
	printRequestToClient(postString, client);

	// wait a bit to get the response
	delay(200);


	// available() will return the number of characters
	// currently in the receive buffer.
	while (client.available())
	Serial.write(client.read()); // read() gets the FIFO char

	// connected() is a boolean return value - 1 if the
	// connection is active, 0 if it's closed.
	if (client.connected())
	client.stop(); // stop() closes a TCP connection.



}

void DataPusher::printRequestToClient(String postString, ESP8266Client client) {

	client.print("POST /update HTTP/1.1\n");
	client.print("Host: api.thingspeak.com\n");
	client.print("Connection: close\n");
	client.print("Content-Type: application/x-www-form-urlencoded\n");
	client.print("Content-Length: ");
	client.print(postString.length());
	client.print("\n\n");
	client.print(postString);
}

void DataPusher::printRequestToSerial(String postString) {

	Serial.print("POST /update HTTP/1.1\n");
	Serial.print("Host: api.thingspeak.com\n");
	Serial.print("Connection: close\n");
	Serial.print("Content-Type: application/x-www-form-urlencoded\n");
	Serial.print("Content-Length: ");
	Serial.print(postString.length());
	Serial.print("\n\n");
	Serial.print(postString);
}

String DataPusher::constructPostString(float field1, float field2, float field3) {

	String postString = "api_key=";
	postString  += apiKey;
	postString  += "&field1=";
	postString  += String(field1);
	postString  += "&field2=";
	postString  += String(field2);
	postString  += "&field3=";
	postString  += String(field3);

	return postString;

}
