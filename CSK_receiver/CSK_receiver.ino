#include <Arduino_APDS9960.h>

// Define RGB thresholds for bit pairs
#define RED_00_LOW 155
#define RED_00_HIGH 170
#define GREEN_00_LOW 115
#define GREEN_00_HIGH 140
#define BLUE_00_LOW 200
#define BLUE_00_HIGH 220

#define RED_01_LOW 240
#define RED_01_HIGH 270
#define GREEN_01_LOW 180
#define GREEN_01_HIGH 210
#define BLUE_01_LOW 260
#define BLUE_01_HIGH 280

#define RED_10_LOW 90
#define RED_10_HIGH 120
#define GREEN_10_LOW 80
#define GREEN_10_HIGH 120
#define BLUE_10_LOW 200
#define BLUE_10_HIGH 240

#define RED_11_LOW 180
#define RED_11_HIGH 210
#define GREEN_11_LOW 60
#define GREEN_11_HIGH 90
#define BLUE_11_LOW 200
#define BLUE_11_HIGH 220

// Separator color range
#define SEP_RED_LOW 420
#define SEP_RED_HIGH 480
#define SEP_GREEN_LOW 200
#define SEP_GREEN_HIGH 240
#define SEP_BLUE_LOW 170
#define SEP_BLUE_HIGH 210

// Light-off state
#define LIGHT_OFF_LOW 0
#define LIGHT_OFF_HIGH 10

#define CHECK_INTERVAL 100 


bool isFirst = true;
bool isSecond = true;

// Store the previous detected range
String previousRange = "Unknown";
String concatenatedOutput = ""; // Variable to store the concatenated output
String sentence = ""; // Variable to store the final sentence


void setup() {
  Serial.begin(57600);
  while (!Serial);

  if (!APDS.begin()) {
    Serial.println("Error initializing APDS-9960 sensor.");
  }

  Serial.println("APDS-9960 initialized. Waiting for data...");
}

void loop() {
  // Wait for color data to be available
  while (!APDS.colorAvailable()) {
    delay(1);
  }

  int r, g, b;
  APDS.readColor(r, g, b);

  // Determine the current range
  String currentRange = detectRange(r, g, b);

  // If the range hasn't changed or if it is "nothing", do nothing
  if (currentRange == previousRange || currentRange == "nothing") {
    return;
  }

  // Update the previous range and print the corresponding value
  previousRange = currentRange;
  Serial.print("Detected: ");
  Serial.println(currentRange);

  // Call the function to manage concatenated output
  manageOutput(currentRange);

  delay(CHECK_INTERVAL); // Wait for the next check
}

// Detect which range the RGB values fall into
String detectRange(int r, int g, int b) {
  if (isColorInRange(r, g, b, RED_00_LOW, GREEN_00_LOW, BLUE_00_LOW, RED_00_HIGH, GREEN_00_HIGH, BLUE_00_HIGH)) {
    return "00";
  } else if (isColorInRange(r, g, b, RED_01_LOW, GREEN_01_LOW, BLUE_01_LOW, RED_01_HIGH, GREEN_01_HIGH, BLUE_01_HIGH)) {
    return "01";
  } else if (isColorInRange(r, g, b, RED_10_LOW, GREEN_10_LOW, BLUE_10_LOW, RED_10_HIGH, GREEN_10_HIGH, BLUE_10_HIGH)) {
    return "10";
  } else if (isColorInRange(r, g, b, RED_11_LOW, GREEN_11_LOW, BLUE_11_LOW, RED_11_HIGH, GREEN_11_HIGH, BLUE_11_HIGH)) {
    return "11";
  } else if (isColorInRange(r, g, b, SEP_RED_LOW, SEP_GREEN_LOW, SEP_BLUE_LOW, SEP_RED_HIGH, SEP_GREEN_HIGH, SEP_BLUE_HIGH)) {
    return "sep";
  } else if (isColorInRange(r, g, b, LIGHT_OFF_LOW, LIGHT_OFF_LOW, LIGHT_OFF_LOW, LIGHT_OFF_HIGH, LIGHT_OFF_HIGH, LIGHT_OFF_HIGH)) {
    return "newline";
  }
  return "nothing";
}

// Check if RGB values are in range for a specific color
bool isColorInRange(int r, int g, int b, int rLow, int gLow, int bLow, int rHigh, int gHigh, int bHigh) {
  return r >= rLow && r <= rHigh && g >= gLow && g <= gHigh && b >= bLow && b <= bHigh;
}

// Function to manage concatenated output
void manageOutput(String currentRange) {
  if (currentRange == "newline") {
    // Print the concatenated output when "newline" is detected
    Serial.print("Concatenated Output: ");
    Serial.println(concatenatedOutput);

    // Parse and process the concatenated output
    parseOutput(concatenatedOutput);

    concatenatedOutput = ""; // Reset the concatenated string
  } else {
    // Append the current range to the concatenated string
    concatenatedOutput += currentRange + " ";
  }
}

// Function to parse the concatenated output
void parseOutput(String output) {
  // Remove all "sep" and spaces from the output
  output.replace("sep ", "");
  output.replace(" ", "");

  // Print the binary string
  Serial.print("Binary String: ");
  Serial.println(output);

  // Convert the binary string to an ASCII character
  char asciiChar = strtol(output.c_str(), NULL, 2);

  // Print the ASCII character
  Serial.print("ASCII Character: ");
  Serial.println(asciiChar);

  // Add the ASCII character to the sentence
  addToSentence(asciiChar);
}

// Function to add a character to the sentence
void addToSentence(char asciiChar) {
  if (isFirst) {
    isFirst = false;
    return; 
  }
  if (isSecond) {
    isSecond = false;
    return; 
  }

  sentence += asciiChar;
  // Print the current sentence
  Serial.print("Current Sentence: ");
  Serial.println(sentence);
}

