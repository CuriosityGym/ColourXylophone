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

int i = 0 ;

int state = 0; // 1 - red, 2 - green, 3 - blue, 0 - ignore

unsigned long int pulseWidth;

int rColor = 0;
int gColor = 0;
int bColor = 0;

void sensorsetup(){
 digitalWrite(s0, LOW);         // Sensor control pins for 
 digitalWrite(s1, HIGH);        // 2% output frequency scaling
 digitalWrite(ledstate, HIGH);  // Turn on the LEDs for illumination
}

void connectionsetup(){
  for(i=0;i<5;i++){             // Send 5 'A' characters over
  bluetooth.write(0x41);        // bluetooth for recognition of device
 }
}

void readred(){
   digitalWrite(s2,LOW);          // Tell the sensor to
   digitalWrite(s3,LOW);          // send red colour values
   pulseWidth=pulseIn(out,LOW);   // Count the number of
   rColor = (pulseWidth/400) - 1; // pulses received from the sensor
   rColor = (255 -rColor);
}

void readgreen(){
   digitalWrite(s2,HIGH);         // Tell the sensor to
   digitalWrite(s3,HIGH);         // send green colour values
   pulseWidth=pulseIn(out,LOW);   // Count the number of
   gColor = (pulseWidth/400) - 1; // pulses received from the sensor
   gColor = (255 -gColor);
}

void readblue(){
   digitalWrite(s2,LOW);          // Tell the sensor to
   digitalWrite(s3,HIGH);         // send blue colour values
   pulseWidth=pulseIn(out,LOW);   // Count the number of 
   bColor = (pulseWidth/400) - 1; // pulses received from the sensor
   bColor = (255 -bColor); 
}

void tunecolours(){
  // Tuning white colour
  if (rColor >=251 && gColor >=251 && bColor >=251)
 {
 rColor = 255;                
 bColor = 255; 
 gColor = 255;
  }  
  // Tuning red color
else  if(rColor > gColor && gColor > bColor)
  {
   rColor = rColor ;           
   gColor=gColor/1.5;  
   bColor=bColor /10;  
  }
  else if(rColor > bColor && bColor > gColor)
  {
   rColor = rColor  ;
   bColor = bColor /8;
   gColor = gColor /10;
  }
  // Tuning green color   
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
  // Tuning blue color
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
}

int getcolourstate(){
  if(rColor>230 && rColor<255 &&  gColor>0 && gColor<25 && bColor>=0 && bColor<20 ){
    return 1;   // Return 1 if the colour found is red
  }
  if(rColor>95 && rColor<125 &&  gColor>200 && gColor<230 && bColor>=0 && bColor<20 ){
    return 2;   // Return 2 if the colour found is green
  }
  if(rColor>5 && rColor<30 &&  gColor>90 && gColor<110 && bColor>155 && bColor<175 ){
    return 3;   // Return 3 if the colour found is blue
  }
  if(rColor>60 && rColor<80 &&  gColor>0 && gColor<30 && bColor>=0 && bColor<25 ){
    return 4;   // Return 4 if the colour found is violet
  }
  return 0;
  /*if(rColor>10 && rColor<30 &&  gColor>110 && gColor<125 && bColor>175 && bColor<190 ){
    state = 5;
  }*/
}

void setup() 
{
 bluetooth.begin(115200);
 //Serial.begin(9600);
 pinMode(s2,OUTPUT);         //Set the sensor control pins as OUTPUT
 pinMode(s3,OUTPUT);         //Set the sensor control pins as OUTPUT
 pinMode(out,INPUT);         //Set the sensor output pins as INPUT
 pinMode(s0, OUTPUT);        //Set the sensor control pins as OUTPUT
 pinMode(s1, OUTPUT);        //Set the sensor control pins as OUTPUT
 pinMode(ledstate, OUTPUT);  //Set the LED control pins as OUTPUT
 
 sensorsetup();              // Call the function to setup the sensor
 
 connectionsetup();
}

void loop() 
{
  readred();         // Read red values from sensor
  readgreen();       // Read green values from sensor
  readblue();        // Read blue values from sensor
   
  tunecolours();     // Tuning the colours for more accurate results.  

  bColor =bColor * .06;
  gColor =gColor * .95;
  //rColor =rColor * .002;  
  
  state = getcolourstate(); // Fetch the colour state to send over bluetooth.
  /*Serial.print("R = ");
  Serial.println(rColor);
  Serial.print("G = ");
  Serial.println(gColor);
  Serial.print("B = ");
  Serial.println(bColor);*/
  switch(state){
   case 0 : bluetooth.write(0x30);  // Send ASCII char 0
            break;
   case 1 : bluetooth.write(0x31);  // Send ASCII char 1
            break;
   case 2 : bluetooth.write(0x32);  // Send ASCII char 2
            break;
   case 3 : bluetooth.write(0x33);  // Send ASCII char 3
            break;
   case 4 : bluetooth.write(0x34);  // Send ASCII char 4
            break;
   default : break;
  }
  delay(50);
}
