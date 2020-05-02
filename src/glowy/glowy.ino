#include <SoftwareSerial.h>
#include <Wire.h>

//DEBUG?
#define DEBUG 0

//ACCELEROMETER SETUP
#define ADX 0x1d //i2c Address
//SDA -> A4
//SCL -> A5

//BLUETOOTH SETUP
#define TX 10 //Connect to TX pin on HC06
#define RX 11 //Connect to RX pin on HC06
#define REQUEST_SIZE 10
SoftwareSerial hc06(TX,RX);
char request[REQUEST_SIZE];

//LED SETUP
#define LED_L_OUT 7
#define LED_R_OUT 5
#define LED_COUNT 6

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
	//Strncmp works best. It's not ideal though
	if (!strncmp(request,"wavy",4))
	{
		wavy();
		return;
	}
	if (!strncmp(request,"popo",4))
	{
		popo();
		return;
	}
	/*
	if (!strncmp(request,"rgb",3))
	{
		rgb();
		return;
	}*/
	if (!strncmp(request,"bump",4))
	{
		bump();
		return;
	}
	if (!strncmp(request,"0",1))
	{
		off();
		return;
	}
	//if (strncmp(request,""))	
	//{}

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
	
	//Start talking to ADX
	Wire.begin();
  	//Access power control register and set it to start measuring
  	Wire.beginTransmission(ADX); // Start communicating
  	Wire.write(0x2D); // Access POWER_CTL Register - 0x2D
  	Wire.write(8); // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable 
  	Wire.endTransmission();
  	delay(10);
  	//Change range to 8g
  	Wire.beginTransmission(ADX);
  	Wire.write(0x31); //This is where the range value lives;
  	Wire.write(2); //0000 0010 8g mode
  	Wire.endTransmission();
  	delay(10);
}  	

void loop(){
	delay(200);
	if (hc06.available())
	{
		req_handle();
	}	
}