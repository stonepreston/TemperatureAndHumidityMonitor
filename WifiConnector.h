#ifndef WifiConnector_H
#define WifiConnector_H

class WifiConnector {

	private:

		// Properties
		String networkName;
		String networkPassword;

	public:

		// Methods
		WifiConnector(String name, String pass);
		void initializeESP8266();
		void connectESP8266();
		void displayConnectionInfo();
		static void errorLoop(int error);

};

#endif
