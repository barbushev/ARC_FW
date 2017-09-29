#include "avr/wdt.h"  //for watch dog

const char FIRMWARE_VERSION[] = "1000";  // will start at 1000 and go up
const uint32_t BAUD_RATE = 9600;
const char FAULT_RESP[] = "FLT";

const uint8_t PIN_RANGE_MIN = 2;   //pins numbers between range_min and range_max will be used
const uint8_t PIN_RANGE_MAX = 19;


char inputBuffer[4];   //incoming commands will be 3 chars at most + null
uint8_t inBufPos = 0;
bool commandComplete = false;

// function prototypes
void clearInputBuffer(void);


void setup() {	
    //set watchdog to 500ms
    wdt_enable(WDTO_500MS);  
    
	//set all pins to output
	for (uint8_t pin = PIN_RANGE_MIN; pin <= PIN_RANGE_MAX; pin++ )
		pinMode(pin, OUTPUT);     

	// Initialize Serial Port	
	Serial.begin(BAUD_RATE);
}


void loop() {
	
	wdt_reset();

	while (Serial.available())
	{
		// get character from serial port
		char inChar = (char)Serial.read();

		// set complete flag is command is done
		if (inChar == '\n')
		{
			inputBuffer[inBufPos] = '\0'; //add null terminator
			commandComplete = true;
			break;
		}
		else
			inputBuffer[inBufPos++] = inChar;

		if (inBufPos >= sizeof(inputBuffer))
		{
			clearInputBuffer();
			Serial.println(FAULT_RESP);
			break;
		}
	}


	if (commandComplete)
	{
		processCommand();
		uint8_t pin = 2;


		switch (inputBuffer[0])
		{
			case 'H': digitalWrite(pin, HIGH);
				break;
			case 'L': digitalWrite(pin, LOW);
				break;

			default:
				Serial.println(FAULT_RESP);
		}


		clearInputBuffer();
	}
}


void processCommand()
{
	uint8_t pin = getPinNumber();

	if (!pinNumberIsValid(pin))
	{
		Serial.println(FAULT_RESP);
		return;
	}

	switch (inputBuffer[0])
	{
	case 'H': 
		digitalWrite(pin, HIGH);
		Serial.println(inputBuffer);
		break;
	case 'L': 
		digitalWrite(pin, LOW);
		Serial.println(inputBuffer);
		break;

	default:
		Serial.println(FAULT_RESP);
	}
}


uint8_t getPinNumber()
{
	return atoi(&inputBuffer[1]);
}


bool pinNumberIsValid(uint8_t pinNum)
{		
	return ((pinNum >= PIN_RANGE_MIN) && (pinNum <= PIN_RANGE_MAX));
}

void clearInputBuffer(void)
{
	memset(inputBuffer, '\0', sizeof(inputBuffer));
	inBufPos = 0;
	commandComplete = false;
}