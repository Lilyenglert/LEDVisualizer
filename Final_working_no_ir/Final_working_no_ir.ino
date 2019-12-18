#include "arduinoFFT.h"
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

// lighting pin
#define PIN 18

#define SAMPLES 128             //Must be a power of 2
#define SAMPLING_FREQUENCY 9000 //Hz, must be less than 10000 due to ADC

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
int radius = 0;
bool up = true;
int r;
int g;
int b;

int scalar = 250;

int bThresh = 50;
int rThresh = scalar;
int gThresh = scalar*3;


int bMax = 500;
int gMax = 1000;
int rMax = 1500;
 
arduinoFFT FFT = arduinoFFT();
 
unsigned int sampling_period_us;
unsigned long microseconds;
 
double vReal[SAMPLES];
double vImag[SAMPLES];

int volTotal;
int freqTotal;

int volStd;
int freqStd;

int freqAvg;
int volAvg;

int peakTotal;
int totalSamples;
int weightedAvg;

void setup() {
    Serial.begin(9600);
 
    sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
    matrix.begin();
    matrix.setTextWrap(false);
    matrix.setBrightness(100);
//    matrix.setTextColor(colors[0]);
}
 
void loop() {
   
    /*SAMPLING*/
    for(int i=0; i<SAMPLES; i++)
    {
        microseconds = micros();    //Overflows after around 70 minutes!
     
        vReal[i] = analogRead(26);
        vImag[i] = 0;
     
        while(micros() < (microseconds + sampling_period_us)){
        }
    }
 
    /*FFT*/
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
 
    /*PRINT RESULTS*/
//    Serial.println(peak);     //Print out what frequency is the most dominant.
    for(int i=0; i<(SAMPLES/2); i++)
    {
        /*View all these three lines in serial terminal to see which frequencies has which amplitudes*/
         
//        Serial.print((i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES, 1);
//        Serial.print(" ");
//        Serial.println(vReal[i], 1);    //View only this line in serial plotter to visualize the bins
    }
    for(int i=2; i<(SAMPLES/2); i++){
      volTotal += vReal[i];
      totalSamples++;
    }
    volAvg = volTotal/totalSamples;
    Serial.println("VOLUME AVG:");
    Serial.println(volAvg);
    
    for(int i=2; i<(SAMPLES/2); i++){
//      Serial.println(vReal[i]);
      volStd += sq(vReal[i] - volAvg);
//      Serial.println(volStd);
    }
    volStd = sqrt(volStd/(totalSamples -1));
//    Serial.println("STD:");
//    Serial.println(volStd);

    for(int i=2; i<(SAMPLES/2); i++){
      if (vReal[i]>=(volAvg+volStd)){
        peakTotal+=vReal[i];
      }
    }
//    Serial.println(peakTotal);
    for(int i=2; i<(SAMPLES/2); i++){
      if (vReal[i]>=(volAvg+volStd)){
        weightedAvg += (((i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES)*(vReal[i]/peakTotal));
      }
    }
//    Serial.println("WeightedAvg:");
//    Serial.println(weightedAvg);
    
    delay(10);

    radius = volumetoRad(volAvg);
    
    matrix.fillScreen(matrix.Color(0, 0, 0));
    colorVals(weightedAvg);
    matrix.fillCircle(16, 16, radius , matrix.Color(r, g, b));
    matrix.show();
  

    peakTotal = 0;
    totalSamples = 0;
    volTotal = 0;
    volStd = 0;
    weightedAvg = 0;
}
    int volumetoRad(int vol) {
      return vol/225;
    }

    void colorVals(int freq) {
    b = max(0,int((freq-bThresh)*(1.0/(scalar))*255));
    g = max(0,int((freq-gThresh)*(1.0/scalar)*255));
    r = max(0,int((freq-rThresh)*(1.0/(scalar*2))*255));

    if(freq>bMax){
      b=0;
    }
    if(freq>rMax){
      b=max(0,int((freq-rMax)*(1.0/(scalar))*255));
    }

    r= min(255,r);
    b= min(255,b);
    g= min(255,g);
//    return (String(r) + ", " + String(g) + ", " + String(b));
};
