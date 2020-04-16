// MotorControl.h

#ifndef _MOTORCONTROL_h
#define _MOTORCONTROL_h

#include "Arduino.h"

class MotorControl{
	
	public:
	MotorControl(int ENA, int ENB, int f_left, int r_left, int f_right, int r_right);
	void forward(int ENA, int ENB, int f_left, int r_left, int f_right, int r_right, int speedL, int speedR);
	void forward_control(int ENA, int ENB, int f_left, int r_left, int f_right, int r_right, int speedL, int speedR);
	void backward(int ENA, int ENB, int f_left, int r_left, int f_right, int r_right, int speedL, int speedR);
	void turnleft(int ENA, int ENB, int f_left, int r_left, int f_right, int r_right , int speedL, int speedR);
	void turnright(int ENA, int ENB, int f_left, int r_left, int f_right, int r_right, int speedL, int speedR);
	void stop();
	int speedcontrol();
	float angle();
	void autodrive(int state);
	const byte encoderPinA;//A pin -> the interrupt pin 5, ?????
	const byte encoderPinB;//B pin -> ?????(??) left
	//???? ????
	const byte encoderPinC;//A pin -> the interrupt pin 4, ?????
	const byte encoderPinD;//B pin -> ?????(??) right
	int encoderPos; // 1920
	int encoderPosL;
	byte encoder0PinALast;
	byte encoder0PinALastL;

	boolean DirectionL = true;
	boolean Direction = true;
	int speed;
	int speedleft;
	
	private:
	int _ENA, _ENB, _f_left, _r_left,  _f_right,  _r_right;

};
#endif

