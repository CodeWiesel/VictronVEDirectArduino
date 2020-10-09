/******************************************************************
 VEDirect Arduino

 Copyright 2018, 2019, Brendan McLearie
 Modified 2020 Jim Smith
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: VEDirect.cpp
 - Implementation
 Updates:
 - 2019-07-14 See VEDirect.h
******************************************************************/

#include "VEDirect.h"

VEDirect::VEDirect(HardwareSerial& port):
	VESerial(port)
	// Initialise the serial port that the
	// VE.Direct device is connected to and
	// store it for later use.
{
}

VEDirect::~VEDirect() {
	// virtual destructor
}

uint8_t VEDirect::begin() {
	// Check connection the serial port
	VESerial.begin(VED_BAUD_RATE);
	if (VESerial) {
		delay(500);
		if(VESerial.available()) {
//			VESerial.flush();  //Actually only flushes the Tx buffer
//			VESerial.end();  //Keep it ON after you have started it.
			return 1;
		}
	}
	return 0;
}

int32_t VEDirect::read(uint8_t target) {
	// Read VE.Direct lines from the serial port
	// Search for the label specified by enum target
	// Extract and return the corresponding value
	// If value is "ON" return 1. If "OFF" return 0;

	uint16_t empty = VED_MAX_READ_LOOPS; //VED_MAX_READ_LOOPS;
	uint16_t loops = VED_MAX_READ_LOOPS;
	uint8_t lines = VED_MAX_READ_LINES;
	int32_t ret = -9999;					// The value to be returned
	char line[VED_LINE_SIZE] = "\0";	// Line buffer
	uint8_t idx = 0;					// Line buffer index
	char* label;
	char* value_str;
	int8_t b;							// byte read from the stream
//for(int i=0; i<10; i++) {
//  Serial.println(ved_labels[i]);
//}

//	VESerial.begin(VED_BAUD_RATE);  //Should be already ON after starting it. Maybe flushing the Rx buffer
	while(VESerial.available()) {  //Flush Rx buffer
		VESerial.read();
	}

	do {  //Wait for start of line (CR, NL)
		b = VESerial.read();
	}
	while (b!='\r');
	
	while ((lines > 0)&&(empty > 0)) {
//Serial.print(lines);
		if (VESerial.available()) {
			while (loops > 0) {
				b = VESerial.read();
//				if ((b == -1) || (b == '\r')) { 	// Ignore '\r' and empty reads
				if ((b == -1) ) { 	// Ignore empty reads
					loops--;
				} else {
					if (idx < VED_LINE_SIZE) {
						if ((b!='\r')&&(b!='\n')) line[idx++] = b;		// Add it to the buffer
					} else {
						return -999;				// Buffer overrun
					}
					if (b == '\r') { 				// start of next line
						break;
					} else {
					}
				}
			}
			line[idx] = '\0';						// Terminate the string

			// Line in buffer
			if (target == VE_DUMP) {
				// Diagnostic routine - just print to Serial0;
				Serial.println(line);
				// Continue on rather than break to reset for next line
			}
/*			
//Serial.println("");
Serial.print(line);
Serial.print("+");
Serial.print(ved_labels[target]);
Serial.print("+");
*/
			label = strtok(line, "\t");  //Label and value separated by tab
			value_str = strtok(0, "\0");  //JWS string is null terminated
/*
if (label==0){
	Serial.print("no label");
}
else {
	Serial.print(label);
}
Serial.print(":");
Serial.print(strlen(ved_labels[target]));
Serial.print("&");
Serial.print(strlen(label));
Serial.print(":");

Serial.print(strcmp(label, ved_labels[target]));
Serial.print(":");

if (value_str==0){
	Serial.println("no value");
}
else {
	Serial.print(value_str);
}
*/
			if (strcmp(label, ved_labels[target]) == 0) {
//				value_str = strtok(0, "\r");  //JWS string is null terminated
				if (value_str[0] == 'O') { 		//ON OFF type
					if (value_str[1] == 'N') {
						ret = 1;	// ON
						break;
					} else {
						ret = 0;	// OFF
						break;
					}
				} else {
					sscanf(value_str, "%ld", &ret);
					break;
				}
			} else {			// Line not of interest
/*			
Serial.print(" not ");
Serial.print(" ");
Serial.print(lines);
Serial.print(" ");
Serial.print(loops);
Serial.print(" ");
Serial.print(empty);
Serial.println(" ");
*/
				lines--;
				loops = VED_MAX_READ_LOOPS;
				line[0] = '\0';
				idx = 0;
			}
		}
		else {
			empty--; //When no serial received just try Max lines times
//Serial.print("No Ser:");
//Serial.println(empty);
			delay(10);
		}
	}
	return ret;
}

void VEDirect::copy_raw_to_serial0() {
	read(VE_DUMP);
}

