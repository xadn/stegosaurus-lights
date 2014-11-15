#include <Adafruit_WS2801.h>
#include "SPI.h" // Comment out this line if using Trinket or Gemma

int dataPin = 2;
int clockPin = 3;

const int numPixelsInStrip = 25;
const int numPixelsUsed = 10;
const int offset = 15;

uint32_t pixels[numPixelsInStrip];
// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
Adafruit_WS2801 strip = Adafruit_WS2801(numPixelsInStrip, dataPin, clockPin);




//Write color to all
void writeColor() {
  int i;
  for (i=offset; i < numPixelsInStrip; i++) {
      strip.setPixelColor(i, pixels[i]);
  }
   strip.show();
}

void makeAllColor(uint32_t c){
  int i;
  for (i=offset; i < numPixelsInStrip; i++) {
      pixels[i]=c;
  }
}

void rotate(){
  uint32_t firstColor = pixels[0];
  int i;
  for (i=1; i<numPixelsInStrip-1; i++){
    pixels[i]=pixels[i+1];

  }
  pixels[numPixelsInStrip-1] = firstColor;
}

void push(uint32_t c){
  rotate();
  pixels[offset+numPixelsUsed-1]=c;
}

/* Helper functions */
// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}


void setup() {
  strip.begin();
  int i;
  for(i=0;i<numPixelsInStrip;i++){
    pixels[i]=0;
  }
//  Serial.begin(9600);
  // Update LED contents, to start they are all 'off'
  strip.show();
}

byte tween(byte startVal, byte endVal, unsigned long cycleTime, unsigned long time){
   double percentThroughCycle = (double)(time % cycleTime) /cycleTime;
   return (byte)(percentThroughCycle * (endVal - startVal)) + startVal;
}


byte sinTween(byte startVal, byte endVal, unsigned long cycleTime, unsigned long time){

   double percentThroughCycle = (double)(time % cycleTime) /cycleTime;
   double valueInTime = (sin(percentThroughCycle*2.0*3.14159)+1)/2;
   return (byte)(valueInTime * (endVal - startVal)) + startVal;
}

void loop() {
  //graduallyChangingLights();
  //chaser2();
  playlist();

}


void playlist(){
  long numberOfAnimations = 3;
  long playLength = 90L * 1000L;
  long timePerAnimation = playLength/numberOfAnimations;
  long time = (millis()%playLength)/timePerAnimation;
  if( time == 0 ){
    rainbow();
  } else if (time == 1) {
    chaser2();
  } else if(time == 2){
    graduallyChangingLights();
  }

}

void chaser(){
  delay(50);
  unsigned long cycleTime = 1000;
  unsigned long time = millis();

  push(Color(
    sinTween(0, 255, cycleTime, time),
    sinTween(0, 255, cycleTime/3, time),
    sinTween(0, 255, cycleTime/4, time)
  ));
  writeColor();
}
int tick = 0;
void chaser2(){
  delay(500);
  tick++;
  if(tick%3){
    push(Color(30,10,00));
  } else {
    push(Color(13,4,22));
  }

  writeColor();
}

void graduallyChangingLights(){
  unsigned long cycleTime = 10000;
  unsigned long time = millis();

  makeAllColor(Color(
    sinTween(0, 20, cycleTime, time),
    sinTween(0, 20, cycleTime/3, time),
    sinTween(0, 20, cycleTime/4, time)
  ));
  writeColor();
}


void rainbow(){
  tick++;
  uint32_t colors[] = {
      Color(30,0,0),
      Color(30,22,0),
      Color(25,25,0),
      Color(0,30,0),
      Color(0,0,30),
      Color(10,0,25),
      Color(20,10,20)
  };
  push(colors[tick%7]);
  writeColor();
  delay(100);



}
