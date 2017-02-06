#ifndef DataPusher_H
#define DataPusher_H

#include <SparkFunESP8266WiFi.h>

class DataPusher {

	public:

		// Methods

		static void push(float field1, float field2, float field3);
		static void printRequestToClient(String postString, ESP8266Client client);
		static void printRequestToSerial(String postString);
		static String constructPostString(float field1, float field2, float field3);

};

#endif
