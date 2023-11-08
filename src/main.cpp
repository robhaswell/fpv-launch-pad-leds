#include <Arduino.h>
#include <FastLED.h>
#include <msp.h>
#include <msptypes.h>

#define NUM_LEDS 32
#define LED_PIN 0
#define DELAY 16

#define SPEED 5

CRGB leds[NUM_LEDS];

// The starting hue of the LED string
uint8_t hue = 0;
uint8_t sat = 255;
uint8_t val = 255;

// The hue increment for each LED
uint8_t hueStep = 255 / NUM_LEDS;

bool recording = false;

MSP msp;

void setup()
{
  // Begin the USB serial connection
  Serial.begin();

  // Initialise LEDs
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);

  Serial2.setTX(8);
  Serial2.setRX(9);
  Serial2.begin(115200);

  leds[0] = CRGB::Blue;
  FastLED.show();
}

void loop()
{
  if (Serial2.available())
  {
    uint8_t c = Serial2.read();
    if (msp.processReceivedByte(c))
    {
      mspPacket_t *packet = msp.getReceivedPacket();
      if (packet->function == MSP_ELRS_BACKPACK_SET_RECORDING_STATE)
        recording = packet->readByte();
      msp.markPacketReceived();
    }
  }

  if (recording)
  {
    for (int i = 0; i < NUM_LEDS; i++)
      leds[i].setHSV(0, sat, sin8(hue * 1.5));
  }
  else
  {
    for (int i = 0; i < NUM_LEDS / 2; i++)
    {
      uint8_t ledHue = hue + i * hueStep;
      leds[i].setHSV(ledHue, sat, val);
    }

    // Mirror the LEDs to the other side
    for (int i = 0; i < NUM_LEDS / 2; i++)
      leds[NUM_LEDS - i - 1].setRGB(leds[i].r, leds[i].g, leds[i].b);
  }
  FastLED.show();
  hue += SPEED;

  delay(DELAY);
}
