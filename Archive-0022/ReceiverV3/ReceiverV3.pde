#include "RGB_Ring_V3.h"
#include <Wire.h>


int S1 = 3; 
int S2 = 4;   
int val1 = 0;     // variable to store the read value
int val2 = 0; 
int turn = 0;

int i;
uint16_t ctr, ctr2;


void setup() { 
#ifdef UART
	InitUART();     
#else
  Serial.begin(19200);           // start serial for output
  Serial.println("hello Disk");
    delay (100);
#endif
	InitIO();	

  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event

  pinMode(S1, INPUT);     
  pinMode(S2, INPUT);
  digitalWrite(S1, HIGH);
  digitalWrite(S2, HIGH);
}

void loop() {
	uint8_t led;
        static int8_t shift = 0;
  val1 =digitalRead(S1);
  val2 =digitalRead(S2);
  if(val1==LOW){  // ================演示代码
    uint16_t ctr;
    ALLLEDRED();
    delay(300);
    ALLLEDYELLO();
    delay(300);
    ALLLEDGREEN();
    delay(300);
    ALLLEDTURQUOISE();
    delay(300);
    ALLLEDBLUE();
    delay(300);
    ALLLEDFUCHSIA();
    delay(300);
    ALLLEDWHITE();
    delay(300);
    ALLLEDBLACK();
    delay(300);

    for (ctr = 0; ctr < 3; ctr++)    {
      sequence ();
    }
    ALLLEDBLACK();	// 测试单个LED
      set_led_rgb(0, 64, 0, 0);
      delay(100);
      set_led_rgb(1, 64, 32, 0);
      delay(100);
      set_led_rgb(2, 64, 64, 0);
      delay(100);
      set_led_rgb(3, 32, 64, 0);
      delay(100);
      set_led_rgb(4, 0, 64, 0);
      delay(100);
      set_led_rgb(5, 0, 64, 32);
      delay(100);
      set_led_rgb(6, 0, 64, 64);
      delay(100);
      set_led_rgb(7, 0, 32, 64);
      delay(100);
      set_led_rgb(8, 0, 0, 64);
      delay(100);
      set_led_rgb(9, 34, 0, 64);
      delay(100);
      set_led_rgb(10, 64, 0, 64);
      delay(100);
      set_led_rgb(11, 64, 0, 32);
      delay(100);
    for (ctr = 0; ctr < 24; ctr++)    {
      rotate(7,CW);
      delay(50);
    }
    
    ALLLEDBLACK();	// 测试滚动LED
    set_led_rgb(3, 64, 0, 64);
    for (ctr = 0; ctr < 60; ctr++)    {
      rotate(1,CW);
      rotate(3,CCW);
      delay(50);
    }
    ALLLEDBLACK();
    ALLLEDYELLO();
    delay(50);
    for (ctr = 0; ctr < 5; ctr++)    {
          swaywobble(50,CW);
    }
          ALLLEDBLACK();
          setwobble(0xFFFF);	
    
    ALLLEDBLACK();
    for (ctr = 0; ctr < 3; ctr++)    {
      fader();
    }
          ALLLEDBLACK();
    for (ctr = 0; ctr < 3; ctr++)    {
      fader_hue ();
    }
          ALLLEDBLACK();
    for (ctr = 0; ctr < 400; ctr++) {
      color_wave (45);
    }
    for(ctr = 0; ctr < 5; ctr++) {				
      disable_timer2_ovf();
      delay(100);
      enable_timer2_ovf();	
      delay(100);
    }

		
	}else if(val2==LOW){	// ==============测试颜色代码
    switch (turn){
      case 1:
        ALLLEDRED();
        break;
      case 2:
        ALLLEDYELLO();
        break;
      case 3:
        ALLLEDGREEN();
        break;
      case 4:
        ALLLEDTURQUOISE();
        break;
      case 5:
        ALLLEDBLUE();
        break;
      case 6:
        ALLLEDFUCHSIA();
        break;
      case 7:
        ALLLEDWHITE();
        break;
      default:
        ALLLEDBLACK();
        break;
    }
    delay(200);
    turn++;
    if(turn==8) turn=0;

	}else{		// ================命令代码	
 
  if(Command[0]=='c'){
 Serial.print("just waitin");
    setwobble(0X0FFF);

    switch (Command[1]){

    case 'i':
      hexConvert();
      if(Command[2]==0x01){
        enable_timer2_ovf();							
      }
      else{							
        disable_timer2_ovf();							
      }
      Command[0]=0;
      break;

    case 'b':	//C2-LED#, C3-R Int, C4-G Int, C5-B Int
      hexConvert();
      concatanate();
      set_led_rgb(Command[2], Command[3], Command[4], Command[5]);
      Command[0]=0;
      break;

    case 'u':	//C2-LED#, C3-RGB, C4-Int 
      hexConvert();
      concatanate();
      set_led_unicolor(Command[2], Command[3], Command[4]);                                       
      Command[0]=0;
      break;

    case 'r':	//C2-R Int, C3-G Int, C4-B Int
      hexConvert();
      concatanate();
      set_all_rgb(Command[2], Command[3], Command[4]);
      Command[0]=0;
      break;

    case 'a':	//C2-RGB, C3-Int
      hexConvert();
      concatanate();
      set_all_unicolor(Command[2], Command[3]);
      Command[0]=0;
      break;
    case 'o':	//C2-color,C3-rotatation times,C4-direction
      hexConvert();
      concatanate();
      colorSelect();
      ctr2 = Command[3];
      ctr2 = ctr2 * 12;
      
      for (ctr = 0; ctr < ctr2; ctr++)    {
        rotate(Command[2],Command[4]);
        delay(50);
      }
       ALLLEDBLACK();
      Command[0]=0;
      break;

    case 'w':	//C2-times to do, C3-Delay length
      hexConvert();
      concatanate();
      ctr=0;
      for(i=0;i<Command[2];i++)   {  
        color_wave(ctr);
        delay (Command[3]);
        ctr=ctr++;
      }
      ALLLEDBLACK();
      clearCommand();
      break;

    case 'x': //C2-times to do, C3-Delay length
      hexConvert();
      concatanate();
      for(i=0;i<Command[2];i++)   {       
        //set_led_unicolor(1, 1, 01);
        random_leds ();
        delay(Command[3]);
        ALLLEDBLACK();
      }

      clearCommand();
      break;

    case 'f': //C2-times to do
      hexConvert();
      concatanate();
      for(i=0;i<Command[2];i++)   {     
        fader();
      }

      clearCommand();
      break;

    case 'h': //C2-times to do
      hexConvert();
      concatanate();
      for(i=0;i<Command[2];i++)   {     
        fader_hue();
      }
      ALLLEDBLACK();
      clearCommand();
      break;

    case'S': //C2-times to do         *NOTE: Capital S
      hexConvert();
      concatanate();
      for(i=0;i<Command[2];i++)    {
        sequence ();
      }
      ALLLEDBLACK();
      clearCommand();
      break;


    case 'y':	//BROKEN
      if(Command[2]>0){
        swaywobble(Command[2],Command[3]);
      }
      else{
        setwobble(0X0FFF);
        Command[1]=0;
        Command[0]=0;
      }					
      break;

    case 't':	// BROKEN
      if(Command[2]>0){
        disable_timer2_ovf();
        delay(Command[2]);
        enable_timer2_ovf();
        delay(Command[2]);
      }
      else{
        enable_timer2_ovf();
        Command[1]=0;
        Command[0]=0;
      }
      break;

    case 'g':	// BROKEN
      if(Command[2]>0){
        rotate(RED, Command[3]);
        rotate(GREEN, Command[4]);
        rotate(BLUE, Command[5]);
        delay(Command[2]);
      }
      else{
        Command[1]=0;
        Command[0]=0;
      }
      break;

    default:
      Command[0]=0;
      break;
    }
  }


  else {
    return;
  }
}
}

void receiveEvent(int howMany)
{
  uint8_t data;
  Serial.println("I received: ");
  ReceivePtr=0;  
  while(1 < Wire.available()) // loop through all but the last
  {
    data = Wire.receive();
    Serial.println(data, HEX);    
    rx_buf[ReceivePtr]=data; // receive byte as a character
    ReceivePtr++;
    if(ReceivePtr==RX_MASK)  ReceivePtr=0;
  }
    data = Wire.receive();    
    Serial.println(data, HEX);
    if(rx_buf[0]=='e'){
        uint8_t l=rx_buf[1];
        Wire.send(brightness[0][l]);
        Wire.send(brightness[1][l]);
        Wire.send(brightness[2][l]);
    } 
    if((ReceivePtr<=COMMAND_SIZE) && (rx_buf[0]==data)) savebuff();
    
}

void hexConvert(){

  for(i=2;i<ReceivePtr;i++)
  {  
    if ((Command[i] >= '0') && (Command[i] <= '9')) {
      Command[i] = Command[i] - '0';
    } 
    else if ((Command[i] >= 'A') && (Command[i] <= 'F')) {
      Command[i] = 10 + Command[i] - 'A';
    } 

  }
  return; 
}

void concatanate(){
  for(i=2;i<ReceivePtr;i++)
  {
    Command[i]= Command[i]*10 +Command[i+1]; 
    i=i++;

  }
  int y= 1;
  for(i=3;i<ReceivePtr;i++)
  {
    Command[i] = Command[i + y];
    y++;
  }  
  return;
}


void colorSelect(){
  switch (Command[2]){
    
   case 0x00: //RED
   set_led_rgb(0,40,0,0);
   delay(1000);
   case 0x01: //GREEN  
   set_led_rgb(0,0,40,0);
   case 0x02: //BLUE
   set_led_rgb(0,0,0,40);
   case 0x03: //YELLOW
   set_led_rgb(0,40,40,0);
   case 0x04: //TURQUOISE
   set_led_rgb(0,0,40,40);
   case 0x05: //FUCHSIA
   set_led_rgb(0,40,0,40);
   case 0x06: //WHITE
   set_led_rgb(0,40,40,40);
    default:
      Command[0]=0;
      break;
  }
}

