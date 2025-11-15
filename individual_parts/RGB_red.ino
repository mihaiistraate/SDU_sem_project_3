#include <Adafruit_NeoPixel.h>

// Define pin connected to the data line of the WS2812B strip
#define PIN_WS2812B 14  // Change to the pin you're using
#define NUM_LEDS 18    // Number of LEDs in the strip

// Create an instance of the Adafruit_NeoPixel library
Adafruit_NeoPixel strip(NUM_LEDS, PIN_WS2812B, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();   // Initialize the LED strip
  strip.show();    // Clear all LEDs
  turnStripRed();  // Turn the strip red
}

void loop() {
  // Nothing to do in the loop
}

// Function to turn the entire strip red
void turnStripRed() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0)); // Full red, no green or blue
  }
  strip.show(); // Apply the color changes to the strip
}
