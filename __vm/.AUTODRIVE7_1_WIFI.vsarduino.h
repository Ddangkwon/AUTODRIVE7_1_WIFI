/* 
	Editor: https://www.visualmicro.com/
			visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
			the contents of the _vm sub folder can be deleted prior to publishing a project
			all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
			note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Arduino Uno WiFi Rev2, Platform=megaavr, Package=arduino
*/

#define __MEGAAVR_ATmega4809__
#define ARDUINO 10809
#define ARDUINO_MAIN
#define F_CPU 16000000L
#define printf iprintf
#define __MEGAAVR__
#define F_CPU 16000000L
#define ARDUINO 10809
#define ARDUINO_AVR_UNO_WIFI_REV2
#define ARDUINO_ARCH_MEGAAVR
#define MILLIS_USE_TIMERB3
//
//
void   turn_r_90();
void   turn_l_90();
void   turn_r_180();
void   turn_l_180();
void wheelSpeedR();
void wheelSpeedL();
int loc_right();
int loc_left();
int loc_backward();
void wifi_loc();
void receiveEvent(int bytes);

#include "pins_arduino.h" 
#include "timers.h" 
#include "arduino.h"
#include "AUTODRIVE7_1_WIFI.ino"
