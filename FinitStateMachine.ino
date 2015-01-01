
#include <SoftwareSerial.h>


typedef struct{
	String		attributeName;
	String		attributeValue;

} Attribute;

typedef struct
{
	String		elementName;
	Attribute*	attributes;
	int			attributeCount;

} Command;



int				statesTable[6][5] = { { 2, 6, 6, 6, 6 },	//WAITING_STATE
						{ 6, 2, 3, 6, 5 },		//READ_ELEMENT_NAME_STATE
						{ 6, 3, 6, 4, 6 },		//READ_ATTRIBUTE_NAME_STATE
						{ 6, 4, 3, 6, 5 },		//READ_ATTRIBUTE_VALUE_STATE
						{ 2, 1, 1, 1, 1 },		//HANDLE_DATA_STATE
						{ 6, 6, 6, 6, 6 } };	//ERROR_STATE



SoftwareSerial	Bluetooth(3, 4); //RX, TX
Command			lastCommand;
int				currentState = 1;
char			lastReadChar;

#define Debug						Serial
#define DEBUG						true


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


int getNextState(){
	int tableColumn;
	int tableRow = currentState - 1;

	if (lastReadChar == '<'){
		tableColumn = 0;
	}
	else if ((lastReadChar >= '0') && (lastReadChar <= '9') ||
		((lastReadChar >= 'A') && (lastReadChar <= 'Z')) ||
		((lastReadChar >= 'a') && (lastReadChar <= 'z'))){
		tableColumn = 1;
	}
	else if (lastReadChar == ' '){
		tableColumn = 2;
	}
	else if (lastReadChar == '='){
		tableColumn = 3;
	}
	else if (lastReadChar == '>'){
		tableColumn = 4;
	}
	else {
		return ERROR_STATE;
	}
	

	debugLog("Getting state frm state table " + tableRow + ' ' + tableColumn);

	return statesTable[tableRow][tableColumn];
}


void act(){

	if (READ_ELEMENT_NAME_STATE == currentState){
		if (lastReadChar == '<'){
			lastCommand.elementName = "";
		}
		else {
			lastCommand.elementName += lastReadChar;
		}
	}
	else if(READ_ATTRIBUTE_NAME_STATE == currentState) {
		if (lastReadChar == ' '){
			lastCommand.attributeCount++;
			lastCommand.attributes[lastCommand.attributeCount - 1].attributeName = "";
		}
		else {
			lastCommand.attributes[lastCommand.attributeCount - 1].attributeName += lastReadChar;
		}
	}
	else if (READ_ATTRIBUTE_VALUE_STATE == currentState){
		if (lastReadChar == '='){
			lastCommand.attributes[lastCommand.attributeCount - 1].attributeValue = "";
		}
		else {
			lastCommand.attributes[lastCommand.attributeCount - 1].attributeValue += lastReadChar;
		}
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
	

	if (Bluetooth.available()){
		debugLog("Bluetooth connected");

		lastReadChar = Bluetooth.read();	//gets character

		currentState = getNextState();		//checks character and get state of it

		//act();								//acts according to current state
	}

}
