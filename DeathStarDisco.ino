#include <FastLED.h>

FASTLED_USING_NAMESPACE

// MSGEQ7 control
#define msg7RESET 3
#define msg7Strobe 2
#define msg7DCout 0

// LED control
#define DATA_PIN    11
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define STRAND_COUNT 7
#define LED_COUNT 7
# define COLOUR_COUNT 7

CRGB colours[COLOUR_COUNT] = { CRGB::Red, CRGB::DarkOrange, CRGB::Yellow, CRGB::LawnGreen, CRGB::Blue, CRGB::Indigo, CRGB::Violet};
CRGB leds[STRAND_COUNT][LED_COUNT];

const int LEDpins[STRAND_COUNT] = {4, 5, 6, 7, 8, 9, 10};
int LEDVolume[STRAND_COUNT] = {1, 2, 3, 4, 5, 6, 7};

void setup()
{
  // Can't use a variable to represent the pin number, hence this mess
  FastLED.addLeds<NEOPIXEL, 4>(leds[0], LED_COUNT);
  FastLED.addLeds<NEOPIXEL, 5>(leds[1], LED_COUNT);
  FastLED.addLeds<NEOPIXEL, 6>(leds[2], LED_COUNT);
  FastLED.addLeds<NEOPIXEL, 7>(leds[3], LED_COUNT);
  FastLED.addLeds<NEOPIXEL, 8>(leds[4], LED_COUNT);
  FastLED.addLeds<NEOPIXEL, 9>(leds[5], LED_COUNT);
  FastLED.addLeds<NEOPIXEL, 10>(leds[6], LED_COUNT);
  setLEDColours();

  pinMode(msg7RESET, OUTPUT);
  pinMode(msg7Strobe, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  getAudioLevels();
  setLEDColours();
}

void getAudioLevels() {
  digitalWrite(msg7RESET, HIGH);
  delay(5);
  digitalWrite(msg7RESET, LOW);

  for (int x = 0 ; x < STRAND_COUNT ; x++)
  {
    digitalWrite(msg7Strobe, LOW);
    delayMicroseconds(35);
    int spectrumRead = analogRead(msg7DCout);
    if (spectrumRead < 70)
      spectrumRead = 0;
    int PWMvalue = map(spectrumRead, 0, 700, 0, 7);

    Serial.print(PWMvalue);
    LEDVolume[x] = PWMvalue;
    Serial.print(", ");
    digitalWrite(msg7Strobe, HIGH);
  }
  Serial.println();
}

void setLEDColours() {
  int c = 0;
  int v = 0;
  for (int s = 0; s < STRAND_COUNT ; s++) {
    v = LEDVolume[s];
    for (int l = 0; l < LED_COUNT; l++) {
      if (l < v) {
        leds[s][l] = colours[l];
      } else {
        leds[s][l] = CRGB::Black;
      }
    }
  }
  FastLED.show();
}
