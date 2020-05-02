//Just to test out some light colors
#include <FastLED.h>

#define LED_COUNT 1
#define max_bright 100;
#define LED_L_OUT 7
CRGB led[LED_COUNT];
bool increment = true;
void setup()
{
	FastLED.addLeds<WS2812,LED_L_OUT,GRB>(led,LED_COUNT);
	fill_solid(led,LED_COUNT,CRGB(253,110,20));
	FastLED.show();
	delay(2000);
}
void loop()
{


}