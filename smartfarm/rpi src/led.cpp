#include"led.h"

void led_init(){
	pinMode(RED, OUTPUT);
	pinMode(GREEN, OUTPUT);
	pinMode(BLUE, OUTPUT);
}

void led_on(){
	digitalWrite(RED, 1);
	digitalWrite(GREEN, 0);
	digitalWrite(BLUE, 0);
}

void led_off(){
	digitalWrite(RED, 0);
	digitalWrite(GREEN, 0);
	digitalWrite(BLUE,0);
}
