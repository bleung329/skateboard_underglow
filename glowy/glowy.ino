#include <FastLED.h>
#include <SoftwareSerial.h>
#include <Wire.h>

//DEBUG?
#define DEBUG 0

//ACCELEROMETER SETUP
#define ADX 0x1d //Address

//BLUETOOTH SETUP
#define TX 10 //Connect to TX pin on HC06
#define RX 11 //Connect to RX pin on HC06
#define REQUEST_SIZE 10
SoftwareSerial hc06(TX,RX);
char request[REQUEST_SIZE];

//LED SETUP
#define LED_COUNT 6
#define LED_L_OUT 7
#define LED_R_OUT 5
//Our addresses
CRGB ledL[LED_COUNT];
CRGB ledR[LED_COUNT];

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
	if (strncmp(request,"wavy",4)==0)
	{
		fill_solid(ledL,LED_COUNT,CRGB(100,0,0));
		FastLED.show();
	}
	if (strncmp(request,"rgb",3))
	{}
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
	//Start bluetooth
	hc06.begin(9600);
	//Establish LED strips
	FastLED.addLeds<WS2812,LED_L_OUT,GRB>(ledL,LED_COUNT);
	FastLED.addLeds<WS2812,LED_R_OUT,GRB>(ledR,LED_COUNT);
	//Start talking to ADX
	Wire.begin();
  	//Access power control register and set it to start measuring
  	Wire.beginTransmission(ADX); // Start communicating
  	Wire.write(0x2D); // Access POWER_CTL Register - 0x2D
  	Wire.write(8); // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable 
  	Wire.endTransmission();
  	delay(10);
}

void loop(){
	delay(1000);
	if (hc06.available())
	{
		req_handle();
	}	
}