#include <Adafruit_NeoPixel.h>

// Pin and NeoPixel configuration
#define PIN 7                // Pin connected to the NeoPixel strip
#define NUMPIXELS 5          // Number of NeoPixels used for bit pairs
#define PERIOD 1000           // Duration for each bit pair display in ms
#define CHAR_DELAY 1000      // Delay between characters in ms

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const char* inputString = "N"; // String to be transmitted

void setup() {
  Serial.begin(115200);
  pixels.begin();  // Initialize the NeoPixel strip
  pixels.clear();  // Ensure all LEDs are off initially
  pixels.show();

  Serial.println("Starting transmission...");
}

void loop() {
  // pixels.clear();
  // pixels.show();
  // delay(300);
  // Loop through each character in the input string
  for (int i = 0; i < strlen(inputString); i++) {
    char currentChar = inputString[i];
    Serial.print("Transmitting Character: ");
    Serial.println(currentChar);

    // Convert the character to its binary representation
    String binaryString = charToBinary(currentChar);
    Serial.print("Binary Representation: ");
    Serial.println(binaryString);

    // Loop through the binary string in bit pairs
    for (int j = 0; j < binaryString.length(); j += 2) {
      String bitPair = binaryString.substring(j, j + 2);
      Serial.print("Sending Bit Pair: ");
      Serial.println(bitPair);
      displayBitPair(bitPair);  // Send the bit pair to the NeoPixel strip
      displaySep();
    }

    // Delay between characters
    pixels.clear();
    pixels.show();
    delay(CHAR_DELAY);
  }

  // Once transmission is complete, turn off the NeoPixels
  pixels.clear();
  pixels.show();
  Serial.println("Transmission complete.");
  while (true); // Stop the loop
}

// Convert a character to an 8-bit binary string
String charToBinary(char c) {
  String binary = "";
  for (int i = 7; i >= 0; i--) {
    binary += ((c >> i) & 1) ? '1' : '0';
  }
  return binary;
}

// Display a bit pair on the NeoPixel strip
void displayBitPair(String bitPair) {
  pixels.clear();  // Clear all NeoPixels

  if (bitPair == "00") {
    pixels.setPixelColor(0, pixels.Color(255, 255, 255)); // Light up Pixel 0
  } else if (bitPair == "01") {
    pixels.setPixelColor(2, pixels.Color(255, 255, 255)); // Light up Pixel 1
  } else if (bitPair == "10") {
    pixels.setPixelColor(4, pixels.Color(255, 255, 255)); // Light up Pixel 2
  } else if (bitPair == "11") {
    pixels.setPixelColor(3, pixels.Color(255, 255, 255)); // Light up Pixel 3
  }

  pixels.show();  // Update the NeoPixel colors
  delay(PERIOD);  // Wait for the duration of the bit pair

}

void displaySep() {
  pixels.clear();  // Clear all NeoPixels
  pixels.setPixelColor(1, pixels.Color(255, 255, 255)); // Light up Pixel 3
  pixels.show();  // Update the NeoPixel colors
  delay(PERIOD);
}
