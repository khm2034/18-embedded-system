detect_humidity : get_humidity.o
	gcc -o detect_humidity main.c get_humidity.o -lwiringPi

get_humidity.o: get_humidity.c
	gcc -O -Wall -c get_humidity.c

clean :
	rm -f get_humidity.o detect_humidity
