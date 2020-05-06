#include <SoftwareSerial.h>
#include <Wire.h>

//DEBUG?
#define DEBUG 0

//BLUETOOTH SETUP
#define TX 4 //Connect to TX pin on HC06
#define RX 2 //Connect to RX pin on HC06
#define REQUEST_SIZE 10
SoftwareSerial hc06(TX,RX);
char request[REQUEST_SIZE];

//LED SETUP
#define LED_COUNT 16
#define LED_L_OUT 12 //Pin for left LED strip
#define LED_R_OUT 13 //Pin for right LED strip

//All the light functions have been stuffed in here
#include "patterns.h"

//Request Handler
void req_handle(){
	static byte ndx = 0;
	char endMarker = '\n';
	char rc;
	while (hc06.available() > 0) 
	{
		rc = hc06.read();
		if (rc != endMarker) 
		{
			request[ndx] = rc;
			ndx++;
			if (ndx >= REQUEST_SIZE) 
			{
				ndx = REQUEST_SIZE - 1;
			}
		}
		else 
		{
			request[ndx] = '\0'; // terminate the string
			ndx = 0;
		}
	}
	#if DEBUG == 1
		Serial.println(request);
	#endif
	//Strncmp works best. It's not ideal though.
	if (!strncmp(request,"wavy",4))
	{
		wavy();
		return;
	}
	if (!strncmp(request,"0",1))
	{
		off();
		return;
	}
}

//MAIN

void setup(){
	//Start debug, if needed
	#if DEBUG == 1
		Serial.begin(9600);
		delay(50);
		Serial.println("Serial active.");
	#endif

	//Establish LED strips
	FastLED.addLeds<WS2812,LED_L_OUT,GRB>(ledL,LED_COUNT);
	FastLED.addLeds<WS2812,LED_R_OUT,GRB>(ledR,LED_COUNT);
	off();

	//Start bluetooth
	hc06.begin(9600);
}  	

void loop(){
	delay(200);
	if (hc06.available())
	{
		req_handle();
	}	
}