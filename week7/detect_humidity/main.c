#include <wiringPi.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <softPwm.h>
#include "get_humidity.h"
#define RGBLEDPOWER 24
#define RED 7
#define GREEN 9
#define BLUE 8
#define FAN 22
#define MOTORCONTROL 23

void red_led(int i)
{
	digitalWrite(RGBLEDPOWER, i);
	digitalWrite(RED, i);
	digitalWrite(BLUE, 0);
	digitalWrite(GREEN, 0);
}

void sig_handler(int signo)
{
	red_led(0);
	digitalWrite(FAN, 0);
	digitalWrite(MOTORCONTROL, 0);
	exit(0);
}

void init()
{
	pinMode(RGBLEDPOWER, OUTPUT);
	digitalWrite(RGBLEDPOWER,1);
	pinMode(RED, OUTPUT);
	pinMode(GREEN, OUTPUT);
	pinMode(BLUE, OUTPUT);
	pinMode(FAN, OUTPUT);
	pinMode(MOTORCONTROL, OUTPUT);
	softPwmCreate(MOTORCONTROL, 0, 100);
}

int main (void)
{
	int humid;
	signal(SIGINT, (void *)sig_handler);
	if (wiringPiSetup() == -1)
		exit(EXIT_FAILURE) ;
	
	if (setuid(getuid()) < 0)
	{
		perror("Dropping privileges failed\n");
		exit(EXIT_FAILURE);
	}
	init();
	while(1)
	{
		while (1) 
		{
			humid = read_dht22_dat();
			if(humid != 0)
				break;
			delay(500); // wait 1sec to refresh
		}
		printf("humid : %d\n", humid);
		if(humid >40)	{
			red_led(1);
			digitalWrite(FAN,1);
			for(int i=0; i<20; i++){
				softPwmWrite(MOTORCONTROL, 5);
				delay(100);
			}
			red_led(0);
			digitalWrite(FAN,0);
			softPwmWrite(MOTORCONTROL, 0);
			delay(5000);
		}		
	}
	return 0;
}
