
#include <SoftwareSerial.h>


typedef struct{
	String		attributeName;
	String		attributeValue

} Attribute;

typedef struct
{
	String		elementName;
	Attribute*	attributes;
	int			attributeCount;

} Command;



SoftwareSerial	Bluetooth(3, 4); //RX, TX
Command			lastCommand;


#define Debug	Serial
#define DEBUG	true


void debugLog(String message){
	if (DEBUG) {
		Debug.println("DEBUG: " + message);
	}
}


void setup()
{
	Bluetooth.begin(9600);

	Debug.begin(57600);
	while (!Debug.available()){
	}
	
	debugLog("The board has started.");
}


void loop()
{
	char lastReadChar;

	if (Bluetooth.available()){
		debugLog("Bluetooth connected");

		lastReadChar = Bluetooth.read();


	}

}
