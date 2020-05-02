#include <FastLED.h>

//A surprise tool we'll use later
void req_handle();

//Our addressables
CRGB ledL[LED_COUNT];
CRGB ledR[LED_COUNT];

//Timing 
unsigned long prev_time = 0;
unsigned long curr_time;

//Options for everything
#define MAX_BRIGHTNESS 100

//Vaporwave mode, flowing pink and blue.
//This is a quick and dirty method for making this sliding animation.
//A continuous linear method is a little more involved.
#define HUE_MIN 175
#define HUE_MAX 240	
#define HUE_DIV (HUE_MAX - HUE_MIN) / LED_COUNT
#define WAVY_PAUSE_TIME 600
#define WAVY_MAX 100
//Something to save me some multiplication time
#define double_LED LED_COUNT*2

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
		if (hc06.available()){return;}

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

//Police Lights
//Self explanatory
#define PO_PAUSE 1000
#define PO_MAX 100

void popo()
{
	bool swap = false;
	while(true){
		curr_time = millis();
		//If new pattern, return and check it out
		if (hc06.available()){return;}
		//wee woo wee woo
		if (curr_time - prev_time >= PO_PAUSE) 
		{
			prev_time = curr_time;
			if (swap)
			{
				fill_solid(ledL,LED_COUNT,CRGB(0,0,PO_MAX));
				fill_solid(ledR,LED_COUNT,CRGB(PO_MAX,0,0));
			}
			else
			{
				fill_solid(ledL,LED_COUNT,CRGB(PO_MAX,0,0));
				fill_solid(ledR,LED_COUNT,CRGB(0,0,PO_MAX));
			}
			swap = !swap;
			FastLED.show();
		}
	}
}

//Bump mode
//Scales brightness of strip according to magnitude of 
//shock on the board.

void bump()
{
	red = map(analogRead(RPot),0,1023,0,BrightMax);
    green = map(analogRead(GPot),0,1023,0,BrightMax);
    blue = map(analogRead(BPot),0,1023,0,BrightMax);
}

void off()
{
	fill_solid(ledL,LED_COUNT,CRGB(0,0,0));
	fill_solid(ledR,LED_COUNT,CRGB(0,0,0));
	FastLED.show();
}