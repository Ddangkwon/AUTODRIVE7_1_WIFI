// HC_SR04.h

#ifndef _HC_SR04_h
#define _HC_SR04_h

#include "Arduino.h"

class HC_SR04{
	
	public:
	double distance;
	HC_SR04(int Echo,int Trig);
	double traveltime;
	double _distance[10];
	double mean_dis;
	double distancemeasure();
	int targetfail_bit;
	int targetfail_return();
	private:
	int _Echo, _Trig;


	
};
#endif