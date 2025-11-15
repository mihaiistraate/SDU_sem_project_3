#include <Adafruit_NeoPixel.h>

// Define pin connected to the data line of the WS2812B strip
#define PIN_WS2812B 14  // Change to your actual pin (e.g., D6)

// Define the number of LEDs in your WS2812B strip
#define NUM_LEDS 18

// Create an instance of the Adafruit_NeoPixel library
Adafruit_NeoPixel strip(NUM_LEDS, PIN_WS2812B, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();   // Initialize the LED strip
  strip.show();    // Clear all LEDs (turn off)
}
int j=0;

void loop() {
  for (int i = 0; i < strip.numPixels(); i++) {
    int pixelHue = (i * 256 / strip.numPixels() + j) & 255; // Calculate hue
    strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue * 65536 / 256)));
  }
  strip.show();  // Update the strip
  ++j;
  if(j==255) j =0;
  delay(10);
}
