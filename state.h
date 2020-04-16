// state_return.h

#ifndef state_h
#define state_h

#include "Arduino.h"
#include "HC_SR04.h"
//object
class state{
	public :
	state();
	int state;
	int return_state(double dis_front, int fail_front,double dis_right90,  int fail_right90, double dis_left90, int fail_left90);
	private:
	int front_bit;
	int right90_bit;
	int left90_bit;

};

#endif



