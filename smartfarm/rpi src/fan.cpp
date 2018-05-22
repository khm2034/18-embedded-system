#include"fan.h"

void fan_init(){
	pinMode(FAN, OUTPUT);
}

void fan_on(){
	digitalWrite(FAN, 1);
}

void fan_off(){
	digitalWrite(FAN, 0);
}
