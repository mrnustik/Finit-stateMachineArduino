
#include <SoftwareSerial.h>


typedef struct{
	String		attributeName;
	String		attributeValue;

} Attribute;

typedef struct
{
	String		elementName;
	Attribute	attributes[2];
	int			attributeCount;       

} Command;



int				statesTable[6][5] = { { 2, 1, 1, 1, 1 },	//WAITING_STATE
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





void debugCommandLog(){
	Serial.println("Debugging last command");
	Serial.print("elementName = ");
	Serial.println(lastCommand.elementName);
	Serial.print("attributesCount = ");
	Serial.println(lastCommand.attributeCount);
        if(lastCommand.attributeCount > 0){
          int i = 0;
          for(i = 0; i < lastCommand.attributeCount;i++){
            Serial.print("attribute position = ");
            Serial.println(i);
            Serial.print("attributeName = ");
            Serial.println(lastCommand.attributes[i].attributeName);
            Serial.print("attributeValue = ");
            Serial.println(lastCommand.attributes[i].attributeValue);
          }
        }
}

void handleData(){
	debugCommandLog();
	//Handle data from your command here.
	//if(lastCommand == "test"){
	//	doSomething();
	//}

	

	
	
	
	currentState = WAITING_STATE; // Handling done now waiting again

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
		return WAITING_STATE;
	}
	

	Serial.print("Getting state frm state table ");
	Serial.print(tableRow);
	Serial.print(' ');
	Serial.println(tableColumn);

	return statesTable[tableRow][tableColumn];
}


void act(){
	Serial.println("Acting");
	if (READ_ELEMENT_NAME_STATE == currentState){
		if (lastReadChar == '<'){
			lastCommand.elementName = "";
		}
		else {
			lastCommand.elementName += lastReadChar;
		}
		Serial.println("currentState = READ_ELEMENT_NAME_STATE");
	}
	else if(READ_ATTRIBUTE_NAME_STATE == currentState) {
		
		if (lastReadChar == ' '){
			lastCommand.attributeCount++;
			lastCommand.attributes[lastCommand.attributeCount - 1].attributeName = "";
		}
		else {
			lastCommand.attributes[lastCommand.attributeCount - 1].attributeName += lastReadChar;
		}
		Serial.println("currentState = READ_ATTRIBUTE_NAME_STATE");
	}
	else if (READ_ATTRIBUTE_VALUE_STATE == currentState){
		if (lastCommand.attributes[lastCommand.attributeCount - 1].attributeName.length()> 0){
			if (lastReadChar == '='){
				lastCommand.attributes[lastCommand.attributeCount - 1].attributeValue = "";
			}
			else {
				lastCommand.attributes[lastCommand.attributeCount - 1].attributeValue += lastReadChar;
			}
			Serial.println("currentState = READ_ATTRIBUTE_VALUE_STATE");
		}
		else{
			currentState = ERROR_STATE;
		}
		
	}
	else if (HANDLE_DATA_STATE == currentState){
		
		if (lastCommand.elementName.length() > 0){
			handleData();
			Serial.println("currentState = HANDLE_DATA_STATE");
		}
		else {
			currentState = ERROR_STATE;
		}
		
	}

}

void setup()
{
	Bluetooth.begin(9600);

	Serial.begin(19200);


	Serial.println("The board is ready.");
}


void loop()
{
	

	if (Bluetooth.available()){
		Serial.println("Bluetooth available");

		lastReadChar = Bluetooth.read();	//gets character
		Serial.print("lastReadChar = ");
                Serial.println(lastReadChar);
  

		currentState = getNextState();		//checks character and get state of it
		Serial.print("currentStatedID = ");
		Serial.println(currentState);

		act();								//acts according to current state
	}

}
