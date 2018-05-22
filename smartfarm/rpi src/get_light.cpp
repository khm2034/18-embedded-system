#include"get_light.h"

int read_mcp3208_adc(unsigned char adcChannel){
	unsigned char buff[3];
	int adcValue = 0;
	pinMode(LIGHT, OUTPUT);
	buff[0] = 0x06 | ((adcChannel & 0x07)>>2);
	buff[1] = ((adcChannel & 0x07)<<6);
	buff[2] = 0x00;

	digitalWrite(LIGHT, 0);
	wiringPiSPIDataRW(SPI_CHANNEL, buff, 3);

	buff[1] = 0x0f & buff[1];
	adcValue = (buff[1]<<8) | buff[2];
	digitalWrite(LIGHT, 1);

	return adcValue;
}
