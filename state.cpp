//
//
//

#include "state.h"
#include "Arduino.h"
#include "HC_SR04.h"
state::state(){
	
	
	front_bit = 0;
	right90_bit = 0;
	left90_bit = 0;
	
}

int state::return_state(double dis_front, int fail_front,double dis_right90, int fail_right90, double dis_left90, int fail_left90){
	
	
	if(dis_front <15){
		front_bit = 10;
	}
	
	else if(dis_front>15){
		front_bit = 0;
	}
	else{
		front_bit = 90;
	}


	if(dis_right90 <30){
		right90_bit = 1;
	}
	
	else if(dis_right90>=30){
		right90_bit = 0;
	}
	else{
		right90_bit = 9;
	}
	
	
	if(dis_left90 <30){
		left90_bit = 100;
	}
	
	else if(dis_left90>=30){
		left90_bit = 0;
	}
	
	else{
		left90_bit =900;
	}
	
	
	state = front_bit + right90_bit + left90_bit ;
	return state;
}