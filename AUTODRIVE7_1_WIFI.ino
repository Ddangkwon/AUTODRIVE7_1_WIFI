#include "WiFiNINA.h"




#include "func.h"
#include "state.h"
#include "MotorControl.h"
#include "mode_r.h"
#include "LSM6DS3SENSOR.h"
#include "HC_SR04.h"
#include "Wire.h"
#include "Arduino.h"
//#include "WiFiNINA.h"
/*
 * AUTODRIVE7_1_WIFI.ino
 *
 * Created: 9/13/2019 4:06:45 PM
 * Author: dram109
 */ 
HC_SR04 left90(14,15);
HC_SR04 left45(18,19);
HC_SR04 front(12,13);
HC_SR04 right45(0,1);
HC_SR04 right90(16,17);


double dis_left90;
double dis_left45;
double dis_front;
double dis_right45;
double dis_right90;

int f_left90;
int f_left45;
int f_front;
int f_right45;
int f_right90;

long turn_state = {0};
long back_state=0;
int t = 0;
int location;
int loc;
const int ENA = 9;

const int ENB = 10;
const int f_left = 6;
const int r_left = 5;
const int f_right = 8;
const int r_right=11;
int a,b;
int motor_check=0;
int temp=0;
int temp_2=0;
const byte encoderPinA = 2;//A pin -> the interrupt pin 5, ?????
const byte encoderPinB = 4;//B pin -> ?????(??) left
//???? ????
const byte encoderPinC = 3;//A pin -> the interrupt pin 4, ?????
const byte encoderPinD = 7;//B pin -> ?????(??) right

volatile int encoderPos = 0; // 1920??? ?????
volatile int encoderPosL = 0;
volatile byte encoder0PinALast;
volatile byte encoder0PinALastL;

boolean DirectionL = true;
boolean Direction = true;
int speed = 100;
int speedleft =95;

LSM6DS3 myIMU(SPI_MODE, 30); //dedicated CS connection (pin D30 or SPIIMU_SS) and a dedicated interrupt pin (SPIIMU_INT),instance
MotorControl motorcontrol(ENA, ENB, f_left,  r_left, f_right, r_right);//instance
state s;//instance
mode_r m;//instance


int state_re;
int mode_re;

int x=10;//wire

char ssid[] = "Ksc";
char pass[] = "dial1513";
int keyIndex = 0;
int reading;
int status = WL_IDLE_STATUS;
WiFiServer server(80);
/*
void wifi_loc();
void wheelSpeedR();
void wheelSpeedL();
int loc_right();
int loc_left();
int loc_backward();
void receiveEvent(int bytes);
*/
void setup()
{

   Wire.begin(4);//A4 SDA A5 SCL GND???? ???? 5V???? ??????=> ??????????? ?????? ?????? ???????? ??? ???? ?????
   Wire.onReceive(receiveEvent); 
   
   Serial.begin(115200);
   /*
   while(status != WL_CONNECTED){
      Serial.print("login...");
      status = WiFi.begin(ssid,pass);
      //delay(1000);
   }
   
   server.begin();
   Serial.print("SSID:");
   Serial.println(WiFi.SSID());
   IPAddress ip = WiFi.localIP();
   Serial.print("IP Address:");
   Serial.println(ip);
   
*/
   pinMode(encoderPinB, INPUT_PULLUP);
   attachInterrupt(encoderPinB, wheelSpeedR, CHANGE);
   //ttachInterrupt(digitalPinToInterrupt(encoderPinA), wheelSpeedR, CHANGE);
   pinMode(encoderPinD, INPUT_PULLUP);
   attachInterrupt(encoderPinD, wheelSpeedL, CHANGE);
   t = 0;
   
   if( myIMU.begin() != 0 )
   
   Serial.println("Device error");
   
   else
   
   Serial.println("Device OK!");
   

   location=1;
   loc=1;
}

void loop()
{
   
   dis_right90 = right90.distancemeasure();
   dis_left90 = left90.distancemeasure();
   dis_front = front.distancemeasure();
   f_front = front.targetfail_return();
   f_right90 = right90.targetfail_return();
   f_left90 = left90.targetfail_return();
   
   state_re = s.return_state(dis_front,f_front,dis_right90,f_right90,dis_left90,f_left90);
   Serial.print("left90 = ");
   Serial.print(dis_left90);
   Serial.print("|  front = ");
   Serial.print(dis_front);
   Serial.print("|  right90 = ");
   Serial.print(dis_right90);
   Serial.println();
   Serial.print("****************** state = ");
   Serial.print(state_re);
   Serial.println("  ************************** ");
   Serial.print("****************** turn = ");
   Serial.print(turn_state);
   Serial.println("  ************************** ");
   Serial.print("t = ");
   Serial.println(t);
   Serial.println();
   Serial.println();
   Serial.println();
   Serial.print("pm2_5 x = ");
   Serial.println(x);

   
   WiFiClient client = server.available();
     if(client)
     {
       //String currentLine = "";
        while(client.connected()){
           if(client.available()){

              char c = client.read();
                    client.print(loc);
                    client.print("0");
                    client.print(x);
                    client.println("X");
                    
                    break;
            
           }
        }
   
     }
    Serial.write(0xff) ;
    Serial.write(loc & 0xff) ;
   switch(state_re){
      case 0      :   //   000
         
         motorcontrol.forward(ENA, ENB, f_left, r_left, f_right, r_right, speedleft, speed);
         
         temp =encoderPos;
         temp_2 =encoderPosL;
          Serial.println("right rotate");
          Serial.println(temp);
          
          Serial.println("left rotate");
          Serial.println(temp_2);
          
          
          delay(100);
          
          Serial.println("right rotate");
          Serial.println(encoderPos);
          
          Serial.println("left rotate");
          Serial.println(encoderPosL);
         Serial.print("motor check");
         Serial.println(motor_check);
          if((encoderPos>=temp-30 && encoderPos<=temp+30)&&(encoderPosL >= temp_2-30 && encoderPosL<temp_2+30)){
             motor_check ++;
             
          }
          else
          {
             motor_check = 0;
          }
		  
           if( motor_check >10){
              motorcontrol.backward(ENA, ENB, f_left, r_left, f_right, r_right, speedleft, speed);
              delay(3000);
			   motor_check=0;
              motorcontrol.stop();
			 
             if (turn_state == 2)
             {
                turn_r_180();
                turn_state = 4;
                
             }
             else if (turn_state == 3)
             {
                turn_l_180();
                turn_state = 5;
             }
             else if (turn_state == 4 )
             {
                dis_right90 = right90.distancemeasure();
                dis_left90 = left90.distancemeasure();
                dis_front = front.distancemeasure();
                f_front = front.targetfail_return();
                f_right90 = right90.targetfail_return();
                f_left90 = left90.targetfail_return();
                
                state_re = s.return_state(dis_front,f_front,dis_right90,f_right90,dis_left90,f_left90);
               if(state_re == 1 ){
                  turn_l_90();
                  turn_state = 3;
               }
               else if (state_re == 100 )
               {
                  turn_r_90();
                  turn_state=2;
               }
               else if (state_re == 0){
                  turn_l_180();
                  turn_state = 5;
               }
               else
               {
                  break;
               }
             }
             else if (turn_state == 5 )
             {
                dis_right90 = right90.distancemeasure();
                dis_left90 = left90.distancemeasure();
                dis_front = front.distancemeasure();
                f_front = front.targetfail_return();
                f_right90 = right90.targetfail_return();
                f_left90 = left90.targetfail_return();
                
                state_re = s.return_state(dis_front,f_front,dis_right90,f_right90,dis_left90,f_left90);
                if(state_re == 1 ){
                   turn_l_90();
                   turn_state = 3;
                }
                else if (state_re == 100 )
                {
                   turn_r_90();
                   turn_state=2;
                }
                else if (state_re == 0){
                   turn_r_180();
                   turn_state = 4;
                }
                else
                {
                   break;
                }
             }
              
           }
           
           break;
      case 1      :   //   001
         motorcontrol.forward(ENA, ENB, f_left, r_left, f_right, r_right, speedleft, speed);
         
         temp =encoderPos;
         temp_2 =encoderPosL;
         delay(100);
		  Serial.print("motor check");
		  Serial.println(motor_check);
         if((encoderPos>=temp-30 && encoderPos<=temp+30)&&(encoderPosL >= temp_2-30 && encoderPosL<temp_2+30)){
            motor_check ++;
            
         }
         else
         {
            motor_check = 0;
         }
         if( motor_check >10){
            motorcontrol.backward(ENA, ENB, f_left, r_left, f_right, r_right, speedleft, speed);
            delay(3000);
			 motor_check = 0;
            motorcontrol.stop();
            delay(100);
            if(turn_state == 5){
               turn_l_90();
               turn_state =3;
            }
            else{
               turn_l_180();
               turn_state = 5;
            }
         }
      
            break;
   
      case 10      :   //   010
         motorcontrol.stop();
         delay(100);
         if (turn_state == 2|| turn_state ==0 || turn_state == 5)
         {
            turn_r_180();
            turn_state =4;
         }
         else if(turn_state == 3 || turn_state == 4)
         {
            turn_l_180();
            turn_state = 5;
         }
         break;
      case 100   :   //   100
         motorcontrol.forward(ENA, ENB, f_left, r_left, f_right, r_right, speedleft, speed);
         
         temp =encoderPos;
         temp_2 =encoderPosL;
         delay(100);
         if((encoderPos>=temp-30 && encoderPos<=temp+30)&&(encoderPosL >= temp_2-30 && encoderPosL<temp_2+30)){
            motor_check ++;
            
         }
         else
         {
            motor_check = 0;
         }
         if( motor_check >10){
            motorcontrol.backward(ENA, ENB, f_left, r_left, f_right, r_right, speedleft, speed);
            delay(3000);
			 motor_check = 0;
            motorcontrol.stop();
            delay(100);
            if(turn_state == 4){
               turn_r_90();
               turn_state =2;
            }
            else{
               turn_r_180();
               turn_state = 4;
            }
         }
         
         break;
         
      case 11   :
         if (turn_state == 3){
            turn_l_180();
            turn_state =5;
         }
         else
         {
            turn_l_90();
            turn_state = 3;
         }
         break;
      case 110   :
         if (turn_state == 2){
            turn_r_180();
            turn_state =4;
         }
         else
         {
            turn_r_90();
            turn_state = 2;
         }
         break;
      case 101   :
         motorcontrol.forward(ENA, ENB, f_left, r_left, f_right, r_right, speedleft, speed);
         
         temp =encoderPos;
         temp_2 =encoderPosL;
         delay(100);
		 Serial.print("motor check");
		 Serial.println(motor_check);
         if((encoderPos>=temp-30 && encoderPos<=temp+30)&&(encoderPosL >= temp_2-30 && encoderPosL<temp_2+30)){
            motor_check ++;
            
         }
         else
         {
            motor_check = 0;
         }
         if( motor_check >10){
         
			 motor_check=0;
            while(state_re == 101 ){
               motorcontrol.backward(ENA, ENB, f_left, r_left, f_right, r_right, speedleft, speed);
               dis_right90 = right90.distancemeasure();
               dis_left90 = left90.distancemeasure();
               dis_front = front.distancemeasure();
               f_front = front.targetfail_return();
               f_right90 = right90.targetfail_return();
               f_left90 = left90.targetfail_return();
               state_re = s.return_state(dis_front,f_front,dis_right90,f_right90,dis_left90,f_left90);
            }
            motorcontrol.stop();
            delay(100);
            if (turn_state == 2)
            {
               turn_r_180();
               turn_state = 4;
               
            }
            else if (turn_state == 3)
            {
               turn_l_180();
               turn_state = 5;
            }
            
            else if(turn_state == 4)
            {
               
               if(state_re == 1 ){
                  turn_l_90();
                  turn_state = 3;
               }
               else
               {
                  turn_l_180();
                  turn_state = 5;
               }
            }
            else if (turn_state == 5){
               if(state_re == 100){
                  turn_r_90();
                  turn_state = 2;
               }
               else
               {
                  turn_r_180();
                  turn_state = 4;
               }
            }
            else
            {
               if(state_re != 1 ){
                  turn_r_180();
                  turn_state = 4;
               }
               else
               {
                  turn_l_90();
                  turn_state = 3;
               }
            }
         }
         break;
      default: // 111   112   121   211   122   212   221   222
	  motorcontrol.backward(ENA, ENB, f_left, r_left, f_right, r_right, speedleft, speed);
	  delay(1000);
      while(state_re == 111 ){
         motorcontrol.backward(ENA, ENB, f_left, r_left, f_right, r_right, speedleft, speed);
         dis_right90 = right90.distancemeasure();
         dis_left90 = left90.distancemeasure();
         dis_front = front.distancemeasure();
         f_front = front.targetfail_return();
         f_right90 = right90.targetfail_return();
         f_left90 = left90.targetfail_return();
         state_re = s.return_state(dis_front,f_front,dis_right90,f_right90,dis_left90,f_left90);
      }
      motorcontrol.stop();
      delay(100);
      if (turn_state == 2)
      {   
         turn_r_180();
         turn_state = 4;
         
      }
      else if (turn_state == 3)
      {
         turn_l_180();
         turn_state = 5;
      }
      
      else if(turn_state == 4)
      {
         
         if(state_re == 1 ){
            turn_l_90();
            turn_state = 3;
         }
         else
         {
            turn_l_180();
            turn_state = 5;
         }
      }
      else if (turn_state == 5){
         if(state_re == 100){
            turn_r_90();
            turn_state = 2;
         }
         else
         {
            turn_r_180();
            turn_state = 4;
         }
      }
      else
      {
         if(state_re != 1 ){
            turn_r_180();
            turn_state = 4;
         }
         else
         {
            turn_l_90();
            turn_state = 3;
         }
      }
   }

}



void   turn_r_90(){
   motorcontrol.stop();
   delay(300);
     
   motorcontrol.turnright(ENA, ENB, f_left, r_left, f_right, r_right, speedleft, speed);
   loc=loc_right();
   wifi_loc();
   Serial.write(0xff) ;
   Serial.write(loc & 0xff) ;
   motorcontrol.stop();
   delay(300);

}
void   turn_l_90(){
      motorcontrol.stop();
      delay(300);
       
       
       motorcontrol.turnleft(ENA, ENB, f_left, r_left, f_right, r_right, speedleft, speed);
       loc=loc_left();
       Serial.println(loc);
       wifi_loc();
       Serial.write(0xff) ;
       Serial.write(loc & 0xff) ;
      motorcontrol.stop();
       delay(300);
       
}

void   turn_r_180(){
	int count_18 = 0; 
   delay(300);
   turn_r_90();
   motorcontrol.forward(ENA, ENB, f_left, r_left, f_right, r_right, speedleft, speed);
    do{
	    count_18++;
    }while(count_18<20);
   delay(3000);
   turn_r_90();
   
}
void   turn_l_180(){
	int count_180 = 0; 
   delay(300);
   turn_l_90();
   motorcontrol.forward(ENA, ENB, f_left, r_left, f_right, r_right, speedleft, speed);
   do{
	   count_180++;
   }while(count_180<20);
   delay(3000);
   turn_l_90();
   
}










void wheelSpeedR()
{
  static volatile int Lstate = digitalRead(encoderPinA);
   if((encoder0PinALast == LOW) && Lstate==HIGH)
   {
     static volatile int val = digitalRead(encoderPinB);
      if(val == LOW && Direction)Direction = false; //Reverse
      else if(val == HIGH && !Direction)Direction = true;  //Forward
   }
   encoder0PinALast = Lstate;
   
   if(!Direction)  encoderPos++;
   else  encoderPos--;
   
   if(encoderPos > 10000)encoderPos = encoderPos - 10000;
   else if(encoderPos < -10000)encoderPos = encoderPos + 10000;
}
void wheelSpeedL()
{
   static volatile int Lstate = digitalRead(encoderPinC);
   if((encoder0PinALastL == LOW) && Lstate==HIGH)
   {
    static volatile int val = digitalRead(encoderPinD);
      if(val == LOW && DirectionL)DirectionL = false; //Reverse
      else if(val == HIGH && !DirectionL)DirectionL = true;  //Forward
   }
   encoder0PinALastL = Lstate;
   
   if(!DirectionL)  encoderPosL++;
   else  encoderPosL--;

   if(encoderPosL > 10000)encoderPosL = encoderPosL - 10000;
   else if(encoderPosL < -10000)encoderPosL = encoderPosL + 10000;
}
int loc_right()
{
   if(location==1)
   location=2;
   else if(location==2)
   location=4;
   else if(location==3)
   location=1;
   else if(location==4)
   location=3;
   return location;
}


int loc_left()
{
   if(location==1)
   location=3;
   else if(location==2)
   location=1;
   else if(location==3)
   location=4;
   else if(location==4)
   location=2;
   return location;
}

int loc_backward()
{
   if(location==1)
   location=4;
   else if(location==2)
   location=3;
   else if(location==3)
   location=2;
   else if(location==4)
   location=1;
   return location;
   
}

void wifi_loc()
{
   
   WiFiClient client = server.available();
        if(client)
        {
           // String currentLine = "";
           while(client.connected()){
              if(client.available()){

                 char c = client.read();
                 client.print(loc);
                 client.print("0");
                 client.print(x);
                 client.println("X");
                 break;
                 
              }
           }
           
        }
            
}


void receiveEvent(int bytes)
{
   while(Wire.available()!=0)
   {
      x = Wire.read();
   }
}






/*
void receiveEvent(int howMany)
{
   while(Wire.available()>1)
   {
      char c = Wire.read();
      Serial.print(c);
   }
   int x = Wire.read();
   Serial.println(x);
}
*/