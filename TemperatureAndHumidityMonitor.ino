// Temperature and Humidity Monitoring Program
// Stone Preston
// Will Burson

#include "DHT.h"
#include "config.h"
#include "WifiConnector.h"
#include "DataPusher.h"

// DHT variables
// what digital pin the DHT is connected to
#define DHTPIN 4
#define DHTTYPE DHT22
// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

// Data
float tempF;
float tempC;
float humidity;

void setup() {
  Serial.begin(9600);

  serialTrigger(F("Press any key to begin."));

  WifiConnector connector(configName, configPass);
  connector.initializeESP8266();
  connector.connectESP8266();
  connector.displayConnectionInfo();

  // Begin taking temperature/humidity readings
  dht.begin();

}

void loop() {

  // try taking a reading
  int success = readSensor();

  if (success == 1) {

	  DataPusher::push(tempC, tempF, humidity);
  }

  // can only post to thingspeak every 15 seconds
  delay(20000);

}

// Sensor Helper functions
int readSensor() {

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  tempC = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  tempF = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(tempF) || isnan(tempC)) {
    Serial.println("Failed to read from DHT sensor!");
    return 0;
  }

  return 1;
}

void printSensorData() {

	// Compute heat index in Fahrenheit (the default)
	float hif = dht.computeHeatIndex(tempF, humidity);
	// Compute heat index in Celsius (isFahreheit = false)
	float hic = dht.computeHeatIndex(tempC, humidity, false);

	Serial.print("Humidity: ");
	Serial.print(humidity);
	Serial.print(" %\t");
	Serial.print("Temperature: ");
	Serial.print(tempC);
	Serial.print(" *C ");
	Serial.print(tempF);
	Serial.print(" *F\t");
	Serial.print("Heat index: ");
	Serial.print(hic);
	Serial.print(" *C ");
	Serial.print(hif);
	Serial.println(" *F");
}

// Serial Helper functions

void serialTrigger(String message)
{
  Serial.println();
  Serial.println(message);
  Serial.println();
  while (!Serial.available())
    ;
  while (Serial.available())
    Serial.read();
}

