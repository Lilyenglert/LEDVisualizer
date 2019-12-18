// current status 12/8: motion working smoothly on 

#include <cmath> 
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "arduinoFFT.h"

// light vars
#define lightPin 18
// set up the matrix-neomatrix(width, height, pin .. other pmts stay the same)
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(33, 32, lightPin,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG, 
  NEO_GRB            + NEO_KHZ800);
const uint16_t colors[] = {matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };
int c_x = 2; // x position of circle
int c_y = 0; // y position of circle
int radius = 2; // radius
int r;
int g;
int b;
int scalar = 600;
int bThresh = 50;
int rThresh = scalar;
int gThresh = scalar*3;
int bMax = 1000;
int gMax = 2000;
int rMax = 3000;

// motion vars
//const int irPin1 = 26;
int dist = 0;
float estimate = 0;
int yVAL = 0;
// for smoothing
int reading = 15;

// sound vars
//arduinoFFT FFT = arduinoFFT();
//#define SAMPLES 128             //Must be a power of 2
#define SAMPLING_FREQUENCY 9000 //Hz, must be less than 10000 due to ADC
unsigned int sampling_period_us;
//unsigned long microseconds;
//double vReal[SAMPLES];
//double vImag[SAMPLES];
//int volTotal;
//int freqTotal;
//int volStd;
//int freqStd;
//int freqAvg;
//int volAvg;
//int peakTotal;
//int totalSamples;
//int weightedAvg;


void setup() {
  // motion set up 
  Serial.begin(9600);
//  pinMode(irPin1, INPUT);
  // lighting
  matrix.begin();
  matrix.fillScreen(matrix.Color(0, 0, 0));
  matrix.setBrightness(10);
  //sound
  sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
}

//float retDist(float volt){
//  return (((70.0/(volt/1000))-6.0));
//}
//
//int retLED(float cm){
//  estimate = ((cm-16.5)/1.609375);
////  return round(estimate); //Theoretically more accurate but the closer to 15 we get the more inaccurate the IR becomes
//  return (estimate);
//}

void loop() {
  // sound
  /*SAMPLING*/
//  for(int i=0; i<SAMPLES; i++)
//  {
//      microseconds = micros();    //Overflows after around 70 minutes!
//   
//      vReal[i] = analogRead(A0);
//      vImag[i] = 0;
//   
//      while(micros() < (microseconds + sampling_period_us)){
//      }
//  }

//  /*FFT*/
//  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
//  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
//  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
//  double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
//
//  Serial.println(peak);     //Print out what frequency is the most dominant.
//  for(int i=2; i<(SAMPLES/2); i++){
//    volTotal += vReal[i];
//    totalSamples++;
//  }
//  // vol Avg is the volume used for circle size
//  volAvg = volTotal/totalSamples;
//  for(int i=2; i<(SAMPLES/2); i++){
//    volStd += sq(vReal[i] - volAvg);
//  }
//  volStd = sqrt(volStd/(totalSamples -1));
//  for(int i=2; i<(SAMPLES/2); i++){
//    if (vReal[i]>=(volAvg+volStd)){
//      peakTotal+=vReal[i];
//    }
//  }
//  //weighted average is frequency used for color 
//  for(int i=2; i<(SAMPLES/2); i++){
//    if (vReal[i]>=(volAvg+volStd)){
//      weightedAvg += (((i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES)*(vReal[i]/peakTotal));
//    }
//  }
//  Serial.println(weightedAvg);
//  peakTotal = 0;
//  totalSamples = 0;
//  volTotal = 0;
//  volStd = 0;
//  weightedAvg = 0;
  
//  radius = volTotal;
  // motion
//  dist= retDist(analogRead(irPin1));
//  yVAL = retLED(dist);
//  Serial.println(yVAL);
//  

  // lighting 
  matrix.fillScreen(0);
//  if (yVAL > reading && yVAL < 33) {
//    reading = reading + 1;
//  } else if (yVAL < reading && yVAL < 33) {
//    reading = reading - 1;
//  }
//  c_y = reading;
//  radius = volumetoRad(volAvg);
//  colorVals(weightedAvg);
  matrix.fillCircle(15, 15, 5, matrix.Color(0, 0, 255));
  matrix.show();
  delay(100);
}

//void colorVals(int freq) {
//    b = max(0,int((freq-bThresh)*(1.0/(scalar))*255));
//    g = max(0,int((freq-gThresh)*(1.0/scalar)*255));
//    r = max(0,int((freq-rThresh)*(1.0/(scalar*2))*255));
//
//    if(freq>bMax){
//      b=0;
//    }
//    if(freq>rMax){
//      b=max(0,int((freq-rMax)*(1.0/(scalar))*255));
//    }
//
//    r= min(255,r);
//    b= min(255,b);
//    g= min(255,g);
////    return (String(r) + ", " + String(g) + ", " + String(b));
//}
//
//int volumetoRad(int vol) {
//  if (vol < 500 ) {
//    return vol / 150; 
//  }else if (vol < 2000) {
//    return vol / 200;
//  } else {
//    return vol / 300;
//  };
//}
