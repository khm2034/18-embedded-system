term_project : get_temperature.o fan.o get_light.o led.o
	g++ -o term_project main.cpp get_temperature.o fan.o get_light.o led.o -lwiringPi -lmysqlclient -pthread

get_temperature.o : get_temperature.cpp
	g++ -O -Wall -c get_temperature.cpp

fan.o : fan.cpp
	g++ -O -Wall -c fan.cpp

get_light.o : get_light.cpp
	g++ -O -Wall -c get_light.cpp

led.o : led.cpp
	g++ -O -Wall -c led.cpp

clean :
	rm -f term_project get_temperature.o get_light.o fan.o led.o
