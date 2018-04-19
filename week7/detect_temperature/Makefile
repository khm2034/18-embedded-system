detect_temperature : get_temperature.o
	gcc -o detect_temperature main.c get_temperature.o -lwiringPi

get_temperature.o : get_temperature.c
	gcc -O -Wall -c get_temperature.c

clean :
	rm -f get_temperature.o detect_temperature
