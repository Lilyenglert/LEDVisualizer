#include "arduinoFFT.h"
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <cmath> 

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

int scalar = 800;

int bThresh = 50;
int rThresh = scalar*2;
int gThresh = scalar*3;


int bMax = 2*scalar;
int gMax = 3*scalar;
int rMax = 5*scalar;
 
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

const int irPin1 = 26;
const int irPin2 = 25;
const int irPin3 = 34;
const int irPin4 = 39;
const int irPin5 = 36;
const int irPin6 = 5;
const int irPin7 = 13;
const int irPin8 = 12;
const int irPin9 = 27;
const int irPin10 = 33;
const int irPin11 = 15;
const int irPin12 = 32;

const int IRS = 12;
int IRsensors[IRS] = {irPin1, irPin2, irPin3, irPin4, irPin5, irPin6, irPin7, irPin8, irPin9, irPin10, irPin11, irPin12};

float estimate = 0;

int yTemp;


void setup() {
    Serial.begin(9600);
 
    sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
    matrix.begin();
    matrix.setTextWrap(false);
    matrix.setBrightness(15);
//    matrix.setTextColor(colors[0]);
      Serial.begin(9600);
    pinMode(irPin1, INPUT);
    pinMode(irPin2, INPUT);
    pinMode(irPin3, INPUT);
    pinMode(irPin4, INPUT);
    pinMode(irPin5, INPUT);
    pinMode(irPin6, INPUT);
    pinMode(irPin7, INPUT);
    pinMode(irPin8, INPUT);
    pinMode(irPin9, INPUT);
    pinMode(irPin10, INPUT);
    pinMode(irPin11, INPUT);
    pinMode(irPin12, INPUT);
    yTemp = 0;

}
 
void loop() {
   
    /*SAMPLING*/
    for(int i=0; i<SAMPLES; i++)
    {
        microseconds = micros();    //Overflows after around 70 minutes!
     
        vReal[i] = analogRead(A0);
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

    //  xAVG, Sum of the IR sensors that are activated
  float xAVG = 0;
// xTOT, the number of sensors activated
  float xTOT = 0;
//xAXIS avg of ir sensors
  float xAXIS = 0;
  float xPROP = 0;
  int LEDcol = 0;

  int yCount = 0;
  float yTOT = 0;
  float yAVG = 0;
  int yAXIS = 0;

  const int numVALS=12;
  int values[numVALS]= {0,0,0,0,0,0,0,0,0,0,0,0};
  
  const int numAVGS = 10;
  int AVGS[numAVGS];

  int avgAVGS=0;
  int stdAVGS=0;
  int totAVGS=0;
  int countAVGS=0;

  for (int i = 0; i<IRS; i++){
//    Serial.println(analogRead(IRsensors[i]));
    
    if (retDist(analogRead(IRsensors[i]))<=(60)){
      xAVG += i+1;
      xTOT++;
    }
  }
  if(xTOT>0){
    xAXIS = xAVG/xTOT;
    xPROP = xAXIS/12;
    LEDcol = round(xPROP*33);
    for (int j=0; j<10; j++){
      for(int k=0; k<IRS; k++){
        values[k]=0;
        if(retDist(analogRead(IRsensors[k]))<=60){
          values[k]=analogRead(IRsensors[k]);
        }
      }
      for (int a=0; a<numVALS; a++){
        if (values[a]>0){
          yTOT+=values[a];
          yCount++;
        }
      }
      AVGS[j] = yTOT/yCount;
      avgAVGS+=AVGS[j];
      delay(10);
    }
    avgAVGS=avgAVGS/10;
    for (int b=0; b<numAVGS; b++){
      stdAVGS+= sq(AVGS[b]-avgAVGS);
    }
    stdAVGS = sqrt(stdAVGS/9);
    for (int c=0; c<numAVGS; c++){
      if (AVGS[c]<(avgAVGS + stdAVGS) and AVGS[c]>(avgAVGS-stdAVGS)){
        totAVGS+= AVGS[c];
        countAVGS++;
      }
    }
    if (countAVGS == 0 or retLED(retDist((totAVGS/countAVGS)))>60){
      yAXIS = yTemp;
    }else{
      yAVG = totAVGS/countAVGS;
      yAXIS = retLED(retDist(yAVG));
      if(((yAXIS-yTemp)<=2) and ((yTemp-yAXIS)<=2)){
        yAXIS = yTemp;
      }
      yTemp = yAXIS;
    }
//    Serial.println("LEDcol");
//    Serial.println(LEDcol);
//    Serial.println("y-axis");
//    Serial.println(yAXIS);
  }
    
    delay(10);

    radius = volumetoRad(volAvg);
    if (radius == 0){
      radius=1;
    }
    
    matrix.fillScreen(matrix.Color(0, 0, 0));
    colorVals(weightedAvg);
    if (LEDcol==0 and yAXIS==0){
      matrix.fillCircle(15, 15, radius , matrix.Color(r, g, b));
    }else{
      matrix.fillCircle(LEDcol, yAXIS, radius, matrix.Color(r,g,b));
    }

    matrix.show();
  

    peakTotal = 0;
    totalSamples = 0;
    volTotal = 0;
    volStd = 0;
    weightedAvg = 0;
}
    int volumetoRad(int vol) {
//      Here
      return (vol-300)/225;
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

float retDist(float volt){
  return (((70/(volt/1000))-6.0));
}


int retLED(float cm){
  estimate = ((cm-16.5)/1.609375);
//  return round(estimate); //Theoretically more accurate but the closer to 15 we get the more inaccurate the IR becomes
  return (estimate);
}
