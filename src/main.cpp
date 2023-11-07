#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS 32
#define LED_PIN 0

CRGB leds[NUM_LEDS];

void setup()
{
  // put your setup code here, to run once:
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(100);
}

void loop()
{
  // put your main code here, to run repeatedly:
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::White;
  }
  FastLED.show();
}
