#include <FastLED.h>

//A surprise tool we'll use later
void req_handle();

//Accel values
float X_out, Y_out, Z_out;

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
#define WAVY_PAUSE_TIME 200
#define WAVY_MAX 70
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

//Police Lights
//Self explanatory
#define PO_PAUSE 1000
#define PO_MAX 30

void popo()
{
	bool swap = false;
	while(true)
	{
		curr_time = millis();
		//If new pattern, return and check it out
		if (hc06.available())
		{
			return;
		}
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
#define BUMP_PAUSE 60

void bump()
{
	int r,g,b;
	while (true)
	{
		curr_time = millis();
		if (hc06.available())
		{
			return;
		}
		if (curr_time - prev_time >= BUMP_PAUSE) 
		{
			prev_time = curr_time;
			Wire.beginTransmission(ADX);
			Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
			Wire.endTransmission(false);
			Wire.requestFrom(ADX, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
			X_out = ( Wire.read()| Wire.read() << 8); // X-axis value
			Y_out = ( Wire.read()| Wire.read() << 8); // Y-axis value
			Z_out = ( Wire.read()| Wire.read() << 8); // Z-axis value
			int sum = sqrt(pow(X_out,2)+pow(Y_out,2)+pow(Z_out,2));
			Serial.println(sum);
			r = map(sum,0,1023,10,MAX_BRIGHTNESS);
			fill_solid(ledL,LED_COUNT,CRGB(r,0,0));
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