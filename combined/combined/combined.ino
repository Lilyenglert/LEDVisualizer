#include <cmath> 
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

// light vars
#define lightPin 18
// set up the matrix-neomatrix(width, height, pin .. other pmts stay the same)
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(6, 32, lightPin,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);
const uint16_t colors[] = {matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };
int c_x = 2; // x position of circle
int c_y = 0; // y position of circle
int radius = 2; // radius

// motion vars
const int irPin1 = 26;
int dist = 0;
float estimate = 0;
int yVAL = 0;
// for smoothing
int reading = 15;

void setup() {
  // motion set up 
  Serial.begin(9600);
  pinMode(irPin1, INPUT);
  // lighting
  matrix.begin();
  matrix.setBrightness(10);
  
}

float retDist(float volt){
  return (((70.0/(volt/1000))-6.0));
}

int retLED(float cm){
  estimate = ((cm-16.5)/1.609375);
//  return round(estimate); //Theoretically more accurate but the closer to 15 we get the more inaccurate the IR becomes
  return (estimate);
}

void loop() {
  // motion
  dist= retDist(analogRead(irPin1));
//  Serial.println(dist);
  yVAL = retLED(dist);
  Serial.println(yVAL);
  // lighting 
  matrix.fillScreen(0);
  if (yVAL > reading && yVAL < 33) {
    reading = reading + 1;
  } else if (yVAL < reading && yVAL < 33) {
    reading = reading - 1;
  }
  c_y = reading;
  matrix.fillCircle(c_x, c_y, radius, matrix.Color(100, 100, 100));
  matrix.show();
  delay(100);
}
