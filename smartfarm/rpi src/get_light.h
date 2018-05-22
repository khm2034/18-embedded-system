#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<wiringPi.h>
#include<wiringPiSPI.h>

#define LIGHT 10
#define SPI_CHANNEL 0
#define SPI_SPEED 1000000

int read_mcp3208_adc(unsigned char adcChannel);
