//
//
//

#include "HC_SR04.h"
#include "Arduino.h"

HC_SR04::HC_SR04(int Echo, int Trig){
	pinMode(Echo, INPUT);
	_Echo = Echo;
	pinMode(Trig, OUTPUT);
	_Trig = Trig;
	_distance[10] = {0};
	targetfail_bit = 0;
}
double HC_SR04::distancemeasure(){


	for(int i = 0 ;  i < 10 ; i ++)
	{
		
		digitalWrite(_Trig,LOW);   //Trig 10us ON and go low
		delayMicroseconds(5);
		digitalWrite(_Trig, HIGH);
		delayMicroseconds(10);
		digitalWrite(_Trig, LOW);
		traveltime = pulseIn(_Echo, HIGH);
		distance = (traveltime * 0.034 ) / 2;
		
		if(distance <= 5)
		{
			targetfail_bit = targetfail_bit + 1 ;
			break;
		}
		
		else
		{
			if(targetfail_bit > 0 )
			targetfail_bit = targetfail_bit - 1;
			_distance[i] = distance;
			delayMicroseconds(2);    //  make delay
		}
	}


	double sum = 0;

	for(int a = 0 ; a < 10 ; a++ )    // sum of _distance

	{
		sum = sum + _distance[a];
	}

	mean_dis = sum / 10 ;        //   mean of distances
	return mean_dis;
}

int HC_SR04::targetfail_return()
{
	return targetfail_bit;
}

