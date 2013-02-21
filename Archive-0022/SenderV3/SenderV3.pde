#include <Wire.h>

byte x = 0;
char incomingByte = 0;
byte buffer[20];
byte length=0,index=0;
byte i;


void setup() { 
  
  Serial.begin(19200);  
  
   Wire.begin(); // join i2c bus (address optional for master)
   

 Wire.beginTransmission(4);   
  buffer[0]='c';
   //Serial.println(buffer[0]);
  buffer[1]=0x0f; //r
    //Serial.println(buffer[1]);
  buffer[2]=0x20; //red g intensity
    //Serial.println(buffer[2]);
  buffer[3]=0x30; //green b 
    //Serial.println(buffer[3]);
  buffer[4]=0x00; //blue r
    //Serial.println(buffer[4]);
  buffer[5]=0xF1;
   //Serial.println(buffer[5]);
 for(i=0;i<6;i++)
 
{
   
   Wire.send(buffer[i]);
 }
  
  
         
   Wire.endTransmission();
 
           // start serial for output

  Serial.println(" Hello dumilanove");

 
}



void loop()
{ 
 delay (100);
 {

  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    Serial.print("incoming: ");
    if(incomingByte== 's'){
          Serial.print("SEND: "); 
          Wire.beginTransmission(4);
          // Wire.send('c');         
          for(i=0;i<index;i++)
          {
           //Serial.print(buffer[i], HEX);   
           Serial.print(buffer[i]);
           Serial.print("\t");
           
            Wire.send(buffer[i]);
          }
          Wire.send(buffer[0]);
          Wire.endTransmission();
          Serial.print("I: ");
          Serial.println(index,DEC);
          index=0;
    }else if(incomingByte=='+'){
          for(i=0;i<20;i++)
          buffer[i]=0;
        index=0;
        Serial.println("CLEAR");
        }
        else{
         buffer[index]=incomingByte;
         Serial.print(incomingByte);
         //Serial.println ("increment index");
         index++;
           Serial.print("\t");
          Serial.println(index, HEX);
          
         
    }
  }
}
 }
