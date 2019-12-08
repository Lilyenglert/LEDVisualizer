// things particular to motion functionality can go here
// try and anticipate where integrations with light code will 
// happen and comment accordingly

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif
const int irPin1 = 33;
const int irPin2 = 15;
const int irPin3 = 32;
const int irPin4 = 39;
const int irPin5 = 36;
const int irPin6 = 4;
const int irPin7 = 4;
const int irPin8 = 4;
const int irPin9 = 4;
const int irPin10 = 4;
const int irPin11 = 4;
const int irPin12 = 4;

#define PIN 18
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(9, 32, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

const int baseDistSubtract = 250;// change in housing
const int irPinS[12] = {irPin1,irPin2,irPin3,irPin4,irPin5,irPin6,irPin7,irPin8,irPin9,irPin10,irPin11,irPin12};
int distArr[12];

float dist1;
int pix1;
float dist2;
int pix2;

void setup() {
  Serial.begin(9600);
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(20);
  matrix.setTextColor(colors[0]);
}

int x    = matrix.width();
int y    = matrix.height();
int pass = 0;
int i = 10; 
int i2 = 4;
int r = 2;
bool up = true;

void loop() {
  for(int i = 0; i< 12;i++){
    dist1 = retDistance(analogRead(irPinS[i]));
    pix1 = distToPix(dist1);
    distArr[i] = pix1;
   }
    dist1 = retDistance(analogRead(irPinS[0]));
    pix1 = distToPix(dist1);
    distArr[0] = pix1;
  matrix.fillScreen(0);
  matrix.fillCircle(4, distArr[0], r+3, matrix.Color(10, 10, 255));
  matrix.fillCircle(4, distArr[0], r+2, matrix.Color(150, 150, 255));
  matrix.fillCircle(4, distArr[0], r, matrix.Color(255, 255, 255));
  Serial.println(analogRead(irPinS[0]));
  matrix.show();
  int in = Serial.parseInt();
  if (in > 0){ i = in;}
  delay(10);
}

float retDistance(float rawAnalog){//returns distance in mm
    rawAnalog = map(rawAnalog, 0, 4095, 0, 33000);
    rawAnalog = rawAnalog/10000;
    if(rawAnalog ==0){
      rawAnalog = .0001;
    }
    float dist = ((70.0/rawAnalog) - 6)*10; //convert from v to mm
    return dist;
}
int distToPix (float dist){ //returns y-axis of pixel from inches distance
  //558.8 mm high
  //pixel every 16.6666667mm
  int pixel = round((dist-baseDistSubtract)/16.7); //probs not right
  if(pixel < 0){
    pixel = 0;
  }
  if(pixel> 33){
    pixel = 100;
  }
  return pixel;
}
