#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS 32
#define LED_PIN 0
#define DELAY 16

#define SPEED 5

CRGB leds[NUM_LEDS];

// The starting hue of the LED string
uint8_t hue = 0;
uint8_t sat = 255;
uint8_t val = 100;

// The hue increment for each LED
uint8_t hueStep = 255 / NUM_LEDS;

#define PACKET_LENGTH    12
int cmdString[PACKET_LENGTH];
bool recording = false;

void getRecordingState(int *cmdString) {
    int index = 0;

    if (!Serial2.available()) {
      return;
    }
    Serial.println("Serial2 available");

    // Look for starting letter 36
    int letter = (int)Serial2.read();
    if (letter != 36) {
      return;
    }
    Serial.println("Found starting letter");
    cmdString[index] = letter;
    index++;

    while (index < PACKET_LENGTH) {
      if (Serial2.available()) {
        int letter = (int)Serial2.read();
        Serial.println(letter);
        cmdString[index] = letter;
        index++;
    }
  }

  Serial.println("Got command string: ");
  for (int i = 0; i < PACKET_LENGTH; i++) {
    Serial.println(cmdString[i]);
  }

  if (cmdString[PACKET_LENGTH - 1] == 74) {
    recording = true;
  } else {
    recording = false;
  }
}


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
  getRecordingState(cmdString);

  if (recording) {
    leds[0] = CRGB::Red;
  } else {
    leds[0] = CRGB::Green;
  }
  FastLED.show();

  delay(DELAY);
}
  // // put your main code here, to run repeatedly:
  // for (int i = 0; i < NUM_LEDS; i++)
  // {
  //   uint8_t ledHue = hue + abs8((15 - i) * hueStep);
  //   // uint8_t ba = hue + (i * hueStep);
  //   leds[i].setHSV(ledHue, sat, val);
  // }
  // FastLED.show();
  // delay(DELAY);

  // // Increment the hue to give the rotating effect
  // hue += SPEED;

  // // if (msp.recv(&packet.recvMessageID, packet.payload, packet.maxSize, &packet.recvSize)) {
  // //   FastLED.show();
  // // }
