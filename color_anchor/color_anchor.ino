#include <Adafruit_NeoPixel.h>

// Pin and NeoPixel configuration
#define PIN 7                // Pin connected to the NeoPixel ring
#define NUMPIXELS 5          // Number of NeoPixels
#define DELAY_TIME 2000      // Time to keep each pixel on (in milliseconds)

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();  // Initialize the NeoPixel ring
  pixels.clear();  // Ensure all LEDs are off initially
  pixels.show();
}

void loop() {
  // Turn off all lights for 2000 ms
  pixels.clear();
  pixels.show();
  delay(DELAY_TIME);

  // Sequentially light up each NeoPixel
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.clear();                                // Clear all pixels
    pixels.setPixelColor(i, pixels.Color(255, 255, 255));  // Set the current pixel to white
    pixels.show();                                // Update the NeoPixel ring
    delay(DELAY_TIME);                            // Wait for 2000 ms
  }
}
