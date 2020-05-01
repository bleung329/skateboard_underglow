#include <FastLED.h>

//A surprise tool we'll use later
void req_handle();

//Our addresses
CRGB ledL[LED_COUNT];
CRGB ledR[LED_COUNT];

//Timing 
unsigned long prev_time = 0;

//Vaporwave mode, a flowing pink and blue
#define HUE_MIN 160
#define HUE_MAX 225
#define HUE_DIV (HUE_MAX - HUE_MIN) / LED_COUNT
#define WAVY_INTERVAL 100
void wavy()
{
	while(true){
		unsigned long curr_time = millis();
		if (hc06.available()){return;}
		if (curr_time - prev_time >= WAVY_INTERVAL) {
    		prev_time = curr_time;
		/*
		for (int i = 0; i < LED_COUNT; ++i)
		{
			ledL[i] = CHSV(HUE_MIN+HUE_DIV*i,255,40);
		}*/
			fill_solid(ledL,LED_COUNT,CRGB(100,0,0));
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