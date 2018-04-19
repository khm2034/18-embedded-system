#include <wiringPi.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "get_temperature.h"
#define PUMP 21

void sig_handler(int signo)
{
	digitalWrite(PUMP, 0);
	exit(0);
}

int main (void)
{
	int temp;
	signal(SIGINT, (void *)sig_handler);
	if (wiringPiSetup() == -1)
		exit(EXIT_FAILURE) ;
	
	if (setuid(getuid()) < 0)
	{
		perror("Dropping privileges failed\n");
		exit(EXIT_FAILURE);
	}
	pinMode(PUMP, OUTPUT);
	while(1){
		while(1){
			temp = read_dht22_dat();
			if(temp != 0) break;
			delay(500);
		}
		printf("Temperature = %d\n", temp);
		if(temp > 22){
			digitalWrite(PUMP, 1);
			delay(2000);
			digitalWrite(PUMP, 0);
			delay(5000);
		}
	}
	return 0;
}
