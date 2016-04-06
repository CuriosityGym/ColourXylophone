/***************************************************************************************

Tutorial:http://www.toptechboy.com/arduino/lesson-15-super-cool-arduino-color-sensor-project/

S0 S1 OUTPUT FREQUENCY SCALING           S2 S3 PHOTODIODE TYPE
L L Power down                            L L Red
L H 2%                                    L H Blue
H L 20%                                   H L Clear (no filter)
H H 100%                                  H H Green

s0=Gnd
s1=VCC


RGB
RBG
GRB
GBR
BRG
BGR

tunned for most of the colors 
******************************************************************************************/
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <SoftwareSerial.h>

SoftwareSerial bluetooth(3,4);
//#define wsled 6
//Adafruit_NeoPixel ring = Adafruit_NeoPixel(16, wsled, NEO_GRB + NEO_KHZ800);
int s2 = 10;  //color sensor
int s3 = 11;  //color sensor
int out = 2;  //color sensor
int s0 = 9;
int s1 = 8;
int ledstate = 7;

int state = 0; // 1 - red, 2 - green, 3 - blue, 0 - ignore

unsigned long int pulseWidth;

int rColor = 0;
int gColor = 0;
int bColor = 0;


int getcolourstate(){
  if(rColor>230 && rColor<255 &&  gColor>0 && gColor<25 && bColor>=0 && bColor<20 ){
    return 1;
  }
  if(rColor>95 && rColor<125 &&  gColor>200 && gColor<230 && bColor>=0 && bColor<20 ){
    return 2;
  }
  if(rColor>5 && rColor<30 &&  gColor>90 && gColor<110 && bColor>155 && bColor<175 ){
    return 3;
  }
  if(rColor>60 && rColor<80 &&  gColor>0 && gColor<30 && bColor>=0 && bColor<25 ){
    return 4;
  }
  return 0;
  /*if(rColor>10 && rColor<30 &&  gColor>110 && gColor<125 && bColor>175 && bColor<190 ){
    state = 5;
  }*/
}

void setup() 
{
 bluetooth.begin(115200);
 Serial.begin(9600);
 //ring.begin();
 //ring.show();
// ring.setBrightness(90);

 pinMode(s2,OUTPUT);
 pinMode(s3,OUTPUT);
 pinMode(out,INPUT);
 pinMode(s0, OUTPUT);
 pinMode(s1, OUTPUT);
 pinMode(ledstate, OUTPUT);
 
 digitalWrite(s0, LOW);
 digitalWrite(s1, HIGH);
 digitalWrite(ledstate, HIGH);
}

void loop() 
{
  //read red color
   digitalWrite(s2,LOW);
   digitalWrite(s3,LOW);
   pulseWidth=pulseIn(out,LOW);
   rColor = (pulseWidth/400) - 1;
   rColor = (255 -rColor);

   //read green color
   digitalWrite(s2,HIGH);
   digitalWrite(s3,HIGH);
   pulseWidth=pulseIn(out,LOW);
   gColor = (pulseWidth/400) - 1;
   gColor = (255 -gColor);

   //read blue color
   digitalWrite(s2,LOW);
   digitalWrite(s3,HIGH);
   pulseWidth=pulseIn(out,LOW);
   bColor = (pulseWidth/400) - 1;
   bColor = (255 -bColor);
  // taking out orignal colors

 /*  Serial.print(rColor);
   Serial.print(",");
   Serial.print(gColor);
   Serial.print(",");
   Serial.println(bColor);
   
   delay(100);
*/

    
////////////////////////////////////////// TUNE THE BELOW GIVEN rColor,gColor,bColor for accurate results.//////////////////////////////////////////
   
  //White Color
  if (rColor >=251 && gColor >=251 && bColor >=251)
 {
 rColor = 255;
 bColor = 255; 
 gColor = 255;
  }
  
  //red color
else  if(rColor > gColor && gColor > bColor)
  {
   rColor = rColor ;
   gColor=gColor/1.5;  // gColor = gColor   ;
   bColor=bColor /10;     //bColor = bColor * 0.1;
  }
  else if(rColor > bColor && bColor > gColor)
  {
   rColor = rColor  ;
   bColor = bColor /8;
   gColor = gColor /10;  //0.50
  }

// green color   
   else  if(gColor > rColor && rColor > bColor)
  {
   gColor = gColor;
   rColor = rColor/2;
   bColor = bColor /10;
  }
  else if(gColor > bColor && bColor > rColor)
  
  {
   gColor = gColor;
   bColor = bColor/2;
   rColor = rColor /10;
  }
  
  //blue color
  else if(bColor > rColor && rColor > gColor)
  
  {
   bColor = bColor ;
   rColor = rColor/3;
   gColor = gColor/10;
  }
 else if(bColor > gColor && gColor > rColor)
  {
   bColor = bColor * 12;
   gColor = gColor/2;
   rColor = rColor/10;
  }
  
  // no Color(black out)

 /*else
  {
rColor=0;
gColor=0;
bColor=0;
  }
*/
  // correcting red,blue,green color
bColor =bColor * .06;
gColor =gColor * .95;
//rColor =rColor * .002; 
   /*Serial.print(rColor);
   Serial.print(",");
   Serial.print(gColor);
   Serial.print(",");
   Serial.println(bColor);*/
   
   //delay(100);
   state = getcolourstate();
   Serial.print("R = ");
   Serial.println(rColor);
   Serial.print("G = ");
   Serial.println(gColor);
   Serial.print("B = ");
   Serial.println(bColor);
   switch(state){
    case 0 : bluetooth.write(0x30);
             break;
    case 1 : bluetooth.write(0x31);
             break;
    case 2 : bluetooth.write(0x32);
             break;
    case 3 : bluetooth.write(0x33);
             break;
    case 4 : bluetooth.write(0x34);
             break;
    default : break;
   }
   delay(500);
}
