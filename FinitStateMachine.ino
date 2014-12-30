
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



int				statesTable[6][5] = { { 2, 6, 6, 6, 6 }, { 6, 2, 3, 6, 5 }, { 6, 3, 6, 4, 6 }, { 6, 4, 3, 6, 5 }, { 1, 1, 1, 1, 1 }, { 6, 6, 6, 6, 6 } };



SoftwareSerial	Bluetooth(3, 4); //RX, TX
Command			lastCommand;
int				currentState;

#define Debug	Serial
#define DEBUG	true


#define WAITING_STATE				1
#define READ_ELEMENT_NAME_STATE		2
#define READ_ATTRIBUTE_NAME_STATE	3
#define READ_ATTRIBUTE_VALUE_STATE	4
#define HANDLE_DATA_STATE			5
#define ERROR_STATE					6

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
