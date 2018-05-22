#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<wiringPi.h>

#define LEDPOWER 24
#define RED 7
#define GREEN 9
#define BLUE 8
void led_init();
void led_on();
void led_off();

