// mode_return.h

#ifndef MODE_R_h
#define MODE_R_h
#include "Arduino.h"
#include "HC_SR04.h"
#include "state.h"


class mode_r{
	
	public:
	mode_r();
	int return_mode(int state, long _turn_state);
	int nothing;
	
};


#endif
