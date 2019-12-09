#include <cmath> 

const int irPin1 = 26;
int dist=0;
float estimate=0;
int yVAL = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(irPin1, INPUT);
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
  // put your main code here, to run repeatedly:
  dist= retDist(analogRead(irPin1));
//  Serial.println(dist);
  yVAL = retLED(dist);
  Serial.println(yVAL);  
}
