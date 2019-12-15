// things particular to light functionality can go here
// try and anticipate where integrations with sound & motion
// code will happen and comment accordingly

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

// lighting pin
#define PIN 18

// 
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(33, 32 , PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(255, 255, 255), matrix.Color(0, 0, 255) };

int x = matrix.width();
int y = matrix.height();
int pass = 0;
int i = 10; 
int i2 = 4;
int r = 30;
bool up = true;


void setup() {
  Serial.begin(9600);
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(100);
//  matrix.setTextColor(colors[0]);
} 


void loop() {
  matrix.fillScreen(matrix.Color(0, 0, 0));
  matrix.fillCircle(15, 15, 4, matrix.Color(10, 10, 255));
//  matrix.fillCircle(20, i, r+2, matrix.Color(150, 150, 255));
//  matrix.fillCircle(20, i, r, matrix.Color(255, 255, 255));
  matrix.show();
//  int in = Serial.parseInt();
//  if (in > 0){ i = in;}
//  i = i+1;
  delay(100);
}
// what does the base do 
// take loudness and translate to size of circle 
// take frequency and turn it to colors 
// warm colors & cool colors 
