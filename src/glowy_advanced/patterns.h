#include <FastLED.h>

//Accel values
float X_out, Y_out, Z_out;

//The addressables
CRGB ledL[LED_COUNT];
CRGB ledR[LED_COUNT];

//Timing 
unsigned long prev_time = 0;
unsigned long prev_rest_time = 0;
unsigned long curr_time;

//Options for everything
#define MAX_BRIGHTNESS 100
#define REST_HANDLE_PAUSE 1000

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
		if (curr_time - prev_rest_time >= REST_HANDLE_PAUSE)
		{
			prev_rest_time = millis();
			rest_handle();
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

//<<Police Light mode>>
//Self explanatory.
#define PO_PAUSE 700
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
		if (curr_time - prev_rest_time >= REST_HANDLE_PAUSE)
		{
			prev_rest_time = millis();
			rest_handle();
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
/*
//<<Bump mode>>
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
			r = map(sum,0,1023,10,100);
			fill_solid(ledL,LED_COUNT,CRGB(r,0,0));
			fill_solid(ledR,LED_COUNT,CRGB(r,0,0));
			FastLED.show();
		}
	}
}
*/
//<<Breathe>>
#define BREATHE_INCREMENT_PERIOD 20
#define BREATHE_MAX 70
void breathe()
{
	int r = 0;
	bool increment = true;
	while (true)
	{
		curr_time = millis();
		if (hc06.available())
		{
			return;
		}
		if (curr_time - prev_rest_time >= REST_HANDLE_PAUSE)
		{
			prev_rest_time = millis();
			rest_handle();
		}
		if (curr_time - prev_time >= BREATHE_INCREMENT_PERIOD) 
		{
			prev_time = millis();
			if (increment){++r;}
			else{r--;}
			if (r>=BREATHE_MAX)
			{
				increment = false;
			}
			if (r<=0)
			{
				increment = true;
			}
			fill_solid(ledL,LED_COUNT,CRGB(r,0,0));
			fill_solid(ledR,LED_COUNT,CRGB(r,0,0));
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