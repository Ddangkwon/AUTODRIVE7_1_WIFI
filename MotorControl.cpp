//
//
//

#include "MotorControl.h"
#include "LSM6DS3SENSOR.h"
#include "Arduino.h"

LSM6DS3 LSM6DS3(SPI_MODE, 30);
MotorControl::MotorControl(int ENA, int ENB, int f_left, int r_left, int f_right, int r_right)
{
	pinMode(ENA,OUTPUT);
	pinMode(ENB,OUTPUT);
	pinMode(f_left,OUTPUT);
	pinMode(r_left,OUTPUT);
	pinMode(f_right,OUTPUT);
	pinMode(r_right,OUTPUT);
	_ENA = ENA;
	_ENB = ENB;
	_f_left = f_left;
	_r_left = r_left;
	_f_right = f_right;
	_r_right = r_right;

}



void MotorControl::forward(int ENA, int ENB, int f_left, int r_left, int f_right, int r_right, int speedL, int speedR)
{
	
	digitalWrite(_f_left, HIGH);
	digitalWrite(_r_left, LOW);
	digitalWrite(_r_right, HIGH);
	digitalWrite(_f_right, LOW);
	analogWrite(_ENA, speedL);
	analogWrite(_ENB, speedR);
	Serial.println("forward");
	
}

void MotorControl::turnleft(int ENA, int ENB, int f_left, int r_left, int f_right, int r_right, int speedL, int speedR)
{	float angleX,angleY,angleZ,angleXc,angleYc,angleZc,angle;
	unsigned long start, finished, elapsed;
	float dt=0.015;
	int speedturn =110;
	Serial.println("left");
	start=millis();


	angleZc = LSM6DS3.readFloatGyroZ()*dt;
	if((angleZc<0.1)&&(angleZc>-0.1)){
		angleZc=0;
	}
	angleZ = angleZ + angleZc;

	delay(10);
	// Calculating dt
	finished=millis();
	elapsed=finished-start;
	dt=elapsed/1000000.0;
	start = elapsed = 0;
	if((angleZ<81)&&(angleZ>=0)){
		while((angleZ<81)&&(angleZ>=0)){
			//start=millis();
			start =micros();
			digitalWrite(_f_left, LOW);
			digitalWrite(_r_left, HIGH);
			digitalWrite(_r_right, LOW);
			digitalWrite(_f_right, LOW);
			analogWrite(_ENA, speedturn);
			analogWrite(_ENB, speedturn);
			
			


			angleZc = LSM6DS3.readFloatGyroZ()*dt;
			if((angleZc<0.1)&&(angleZc>-0.1)){
				angleZc=0;
			}
			angleZ = angleZ + angleZc;
			// Serial.print("   angleZ= ");
			// Serial.println(angleZ);
			
			delay(10);
			// Calculating dt
			finished=micros();
			//finished=millis();
			elapsed=finished-start;
			dt=elapsed/1000000.0;
			start = elapsed = 0;
		
			
			//break;
		}

	}
		if(angleZ>81){
			angle=angleZ;
			digitalWrite(_f_left, LOW);
			digitalWrite(_r_right, LOW);
			digitalWrite(_f_right, LOW);
			digitalWrite(_r_left, LOW);
			angleZ=angle-81;
			dt=0;
			//break;
		}
}


void MotorControl::turnright(int ENA, int ENB, int f_left, int r_left, int f_right, int r_right, int speedL, int speedR)
{
	float angleX,angleY,angleZ,angleXc,angleYc,angleZc,angle;
	unsigned long start, finished, elapsed;
	float dt=0.015;
	int speedturn = 110;
	Serial.println("right");
	start=micros();


	angleZc = LSM6DS3.readFloatGyroZ()*dt;
	if((angleZc<0.1)&&(angleZc>-0.1)){
		angleZc=0;
	}
	angleZ = angleZ + angleZc;
	//Serial.print("   angleZ= ");
	//Serial.println(angleZ);

	delay(10);
	// Calculating dt
	finished=micros();
	elapsed=finished-start;
	dt=elapsed/1000000.0;
	start = elapsed = 0;
	if((angleZ>-82)&&(angleZ<=1)){
		while((angleZ>-82)&&(angleZ<=1)){
			digitalWrite(_f_left, LOW);
			digitalWrite(_r_left, LOW);
			digitalWrite(_r_right, LOW);
			digitalWrite(_f_right, HIGH);
			analogWrite(_ENA, speedturn);
			analogWrite(_ENB, speedturn);
			
			start=micros();


			angleZc = LSM6DS3.readFloatGyroZ()*dt;
			if((angleZc<0.1)&&(angleZc>-0.1)){
				angleZc=0;
			}
			angleZ = angleZ + angleZc;
			//Serial.print("   angleZ= ");
			//Serial.println(angleZ);
			
			delay(10);
			// Calculating dt
			finished=micros();
			elapsed=finished-start;
			dt=elapsed/1000000.0;
			start = elapsed = 0;
			
			
			//break;
			
		}
		
	}
	if(angleZ<-82){
		angle=angleZ;
		digitalWrite(_f_left, LOW);
		digitalWrite(_r_right, LOW);
		digitalWrite(_f_right, LOW);
		digitalWrite(_r_left, LOW);
		angleZ=angle+82;
		//break;
	}
}

void MotorControl::backward(int ENA, int ENB, int f_left, int r_left, int f_right, int r_right, int speedL, int speedR) // ????
{	
	digitalWrite(_r_left, HIGH);
	digitalWrite(_f_left, LOW);
	digitalWrite(_f_right, HIGH);
	digitalWrite(_r_right, LOW);
	analogWrite(_ENA, speedL);
	analogWrite(_ENB, speedR);
	Serial.println("Backward");
}
void MotorControl::stop() // ????
{
	digitalWrite(_r_left, LOW);
	digitalWrite(_f_left, LOW);
	digitalWrite(_f_right, LOW);
	digitalWrite(_r_right, LOW);
	Serial.println("stop");
}

void MotorControl::forward_control(int ENA, int ENB, int f_left, int r_left, int f_right, int r_right, int speedL, int speedR)
{
	float angleX,angleY,angleZ,angleXc,angleYc,angleZc;
	unsigned long start, finished, elapsed;
	float dt=0.015;
	digitalWrite(_f_left, HIGH);
	digitalWrite(_r_left, LOW);
	digitalWrite(_r_right, HIGH);
	digitalWrite(_f_right, LOW);
	analogWrite(_ENA, speedL);
	analogWrite(_ENB, speedR);

	
	angleZc = LSM6DS3.readFloatGyroZ();
	if((angleZc<0.1)&&(angleZc>-0.1)){
		angleZc=0;
	}

	delay(10);
	// Calculating dt
	
	
	Serial.println("forward_control");

	
	if((angleZc<10)&&(angleZc>0)){
		while((angleZc<10)&&(angleZc>0)){
			digitalWrite(_f_left, HIGH);
			digitalWrite(_r_left, LOW);
			digitalWrite(_r_right, HIGH);
			digitalWrite(_f_right, LOW);
			analogWrite(_ENA, speedL);
			analogWrite(_ENB, speedR);
			


			angleZc = LSM6DS3.readFloatGyroZ();
			if((angleZc<0.1)&&(angleZc>-0.1)){
				angleZc=0;
			}
			
			// Serial.print("   angleZ= ");
			// Serial.println(angleZ);
			
			delay(10);
			// Calculating dt

			if (angleZc>10)
			{
				speedL = speedL + 1;
				digitalWrite(_f_left, HIGH);
				digitalWrite(_r_left, LOW);
				digitalWrite(_r_right, HIGH);
				digitalWrite(_f_right, LOW);
				analogWrite(_ENA, speedL);
				analogWrite(_ENB, speedR);
				if(speedL==150)
				speedL=110;
				angleZ=0;
				break;
			}
			
			
			
			//break;
		}

	}
	if((angleZc>-10)&&(angleZc<=0.05)){
		while((angleZc>-10)&&(angleZc<=0.05)){
			
			


			angleZc = LSM6DS3.readFloatGyroZ();
			if((angleZc<0.1)&&(angleZc>-0.1)){
				angleZc=0;
			}
			
			// Serial.print("   angleZ= ");
			// Serial.println(angleZ);
			delay(10);
			// Calculating dt
			
			if (angleZc<-10)
			{
				speedR = speedR + 1;
				digitalWrite(_f_left, HIGH);
				digitalWrite(_r_left, LOW);
				digitalWrite(_r_right, HIGH);
				digitalWrite(_f_right, LOW);
				analogWrite(_ENA, speedL);
				analogWrite(_ENB, speedR);
				if(speedR==150)
				speedR=100;
				angleZc=0;
				break;
			}
			//break;
		}

	}
	
}
int MotorControl::speedcontrol(){
	float gap = abs(encoderPosL) - abs(encoderPos); // 왼쪽 바퀴와 오른쪽 바퀴 스피드를 비교
	
	if(gap > 10){ // 왼쪽바퀴가 더 빠름
		Serial.println("fast");
		speedleft = speed * (100 - abs(gap)/10) / 100; //속도 줄이기
	}
	else if(gap < -10){ // 왼쪽바퀴가 더 느림
		Serial.println("slow");
		speedleft = speed * (100 + abs(gap)/10) / 100; //속도 높이기
	}

	//왼쪽바퀴 스피드 수정
	return speedleft;
	

	//밑에는 비교
	Serial.print("   encoderPos : ");
	Serial.println(encoderPos);
	Serial.print("   encoderPosL : ");
	Serial.println(encoderPosL);
	Serial.print("   fixedSpeedL : ");
	Serial.println(speedleft);
	Serial.println("----------------");
}


