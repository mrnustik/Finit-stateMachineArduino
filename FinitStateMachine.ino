
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
									{ 1, 1, 1, 1, 1 },		//HANDLE_DATA_STATE
									{ 6, 6, 6, 6, 6 } };	//ERROR_STATE



SoftwareSerial	Bluetooth(3, 4); //RX, TX
Command			lastCommand;
int				currentState = 1;
char			lastReadChar;

#define Debug						Serial
#define DEBUG_MODE					true


#define WAITING_STATE				1
#define READ_ELEMENT_NAME_STATE		2
#define READ_ATTRIBUTE_NAME_STATE	3
#define READ_ATTRIBUTE_VALUE_STATE	4
#define HANDLE_DATA_STATE			5
#define ERROR_STATE					6



void debugLog(String message){
	if (DEBUG_MODE) {
		Debug.println("DEBUG: " + message);
	}
}

void handleData(){
	
	//Handle data from your command here.
	//if(lastCommand == "test"){
	//	doSomething();
	//}
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
	debugLog("Acting");
	if (READ_ELEMENT_NAME_STATE == currentState){
		if (lastReadChar == '<'){
			lastCommand.elementName = "";
		}
		else {
			lastCommand.elementName += lastReadChar;
		}
		debugLog("currentState = READ_ELEMENT_NAME_STATE");
	}
	else if(READ_ATTRIBUTE_NAME_STATE == currentState) {
		
		if (lastReadChar == ' '){
			lastCommand.attributeCount++;
			lastCommand.attributes[lastCommand.attributeCount - 1].attributeName = "";
		}
		else {
			lastCommand.attributes[lastCommand.attributeCount - 1].attributeName += lastReadChar;
		}
		debugLog("currentState = READ_ATTRIBUTE_NAME_STATE");
	}
	else if (READ_ATTRIBUTE_VALUE_STATE == currentState){
		if (lastCommand.attributes[lastCommand.attributeCount - 1].attributeName.length > 0){
			if (lastReadChar == '='){
				lastCommand.attributes[lastCommand.attributeCount - 1].attributeValue = "";
			}
			else {
				lastCommand.attributes[lastCommand.attributeCount - 1].attributeValue += lastReadChar;
			}
			debugLog("currentState = READ_ATTRIBUTE_VALUE_STATE");
		}
		else{
			currentState = ERROR_STATE;
		}
		
	}
	else if (HANDLE_DATA_STATE == currentState){
		
		if (lastCommand.elementName.length > 0){
			handleData();
			debugLog("currentState = HANDLE_DATA_STATE");
		}
		else {
			currentState = ERROR_STATE;
		}
		
	}

}

void setup()
{
	Bluetooth.begin(9600);

	Debug.begin(57600);
	while (!Debug.available()){
	}
	
	debugLog("The board is ready.");
}


void loop()
{
	

	if (Bluetooth.available()){
		debugLog("Bluetooth connected");

		lastReadChar = Bluetooth.read();	//gets character
		debugLog("Character read");

		currentState = getNextState();		//checks character and get state of it
		debugLog("currentStatedID = " + currentState);

		act();								//acts according to current state
	}

}
