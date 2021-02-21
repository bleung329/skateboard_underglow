#include <SoftwareSerial.h>

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

#include <FastLED.h>

//The addressables
CRGB ledL[LED_COUNT];
CRGB ledR[LED_COUNT];

//Timing 
unsigned long prev_time = 0;
unsigned long curr_time;

//Options for everything
#define MAX_BRIGHTNESS 100

//<<Vaporwave mode, flowing pink and blue.>>
//This is a quick and dirty method for making this sliding animation.
//A continuous/linear method is a little more involved.
#define HUE_MIN 175
#define HUE_MAX 240  
#define HUE_DIV (HUE_MAX - HUE_MIN) / LED_COUNT
#define WAVY_PAUSE_TIME 110
#define WAVY_MAX 85
#define double_LED LED_COUNT*2 //Something to save me some multiplication time

void wavy()
{
  int waviness_index = 0;
  CRGB waviness[double_LED];  
  for (int i = 0; i<LED_COUNT; ++i)
  {
    waviness[i] = CHSV(HUE_MIN+HUE_DIV*i,255,WAVY_MAX);
  }
  for (int i = LED_COUNT; i<double_LED; ++i)
  {
    waviness[i] = CHSV(HUE_MAX-HUE_DIV*(i-LED_COUNT),255,WAVY_MAX);
  }
  while(true){
    curr_time = millis();
    //If new pattern, return and check it out
    if (hc06.available())
    {
      return;
    }
    //If enough time has passed, update the pattern
    if (curr_time - prev_time >= WAVY_PAUSE_TIME) 
    {
      prev_time = curr_time;
      int wav_temp = waviness_index;
      //Select the proper pattern, window style
      for (int i = 0; i < LED_COUNT; ++i)
      {
        ++wav_temp;
        if (wav_temp>=double_LED){
          wav_temp-=double_LED;
        }
        ledL[i] = waviness[wav_temp];
        ledR[i] = waviness[wav_temp];
      }
      ++waviness_index;
      if (waviness_index>=double_LED)
      {
        waviness_index-=double_LED;
      }
      FastLED.show();
    }
  }
}

void off()
{
  fill_solid(ledL,LED_COUNT,CRGB(0,0,0));
  fill_solid(ledR,LED_COUNT,CRGB(0,0,0));
  FastLED.show();
}

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
