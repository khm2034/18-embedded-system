#include<stdio.h>
#include<wiringPi.h>
#include<stdint.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<signal.h>
#include<mysql/mysql.h>
#include<errno.h>
#include<time.h>
#include<string.h>
#include "get_temperature.h"
#include "fan.h"
#include "get_light.h"
#include "led.h"
#define MAX 10000
#define DBHOST "115.68.229.127"
#define DBUSER "root"
#define DBPASS "sqlrootpw"
#define DBNAME "iotfarm"
typedef struct data_s{
	int temp;
	int light;
	char time[30];
}data;
data buffer[MAX];
int fill_ptr = 0, use_ptr = 0;
int count = 0;
int is_fan_on = 0, is_led_on = 0, is_send_data = 0;
int thread_loop = 1;
int id = 0;
time_t timer;
struct tm *t;
pthread_t p1, p2, p3, p4;
pthread_cond_t empty, fill, fan, led;
pthread_mutex_t mutex;
MYSQL *connector;
MYSQL_RES *result;
MYSQL_ROW row;
void sig_handler(int signo){
	thread_loop = 0; is_led_on = is_fan_on = is_send_data = 1;
	pthread_cond_signal(&led); 
	pthread_cond_signal(&fan);
	pthread_cond_signal(&fill);
	led_off(); printf("led off\n");
	fan_off(); printf("fan off\n");
}
void put(int temp, int light) {
	printf("input value temperature : %d\n", temp);
	printf("input value brightness : %d\n", light);
	buffer[fill_ptr].temp = temp;
	buffer[fill_ptr].light = light;
	sprintf(buffer[fill_ptr].time, "'%04d-%02d-%02d %02d:%02d:%02d'", t->tm_year + 1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
	fill_ptr = (fill_ptr+1)%MAX;
	count++;
}
void get(){
	char query[1024];	
	printf("query value temperature : %d\n", buffer[use_ptr].temp);
	printf("query value brightness : %d\n", buffer[use_ptr].light);
	sprintf(query, "insert into farm values (%d, %d, %d, %s)",id++, buffer[use_ptr].light, buffer[use_ptr].temp, buffer[use_ptr].time);
	use_ptr = (use_ptr +1) % MAX;
	count--; printf("count : %d\n", count);
	if(mysql_query(connector, query)){
		fprintf(stderr, "%s\n", mysql_error(connector));
		printf("write db error\n");
	}
}

void *led_thread_func(void *arg){
	while(thread_loop){
		pthread_mutex_lock(&mutex);
		while(!is_led_on) pthread_cond_wait(&led, &mutex);
		led_on();		
		pthread_mutex_unlock(&mutex);
	}
	led_off();
}
void *fan_thread_func(void *arg){
	while(thread_loop){
		pthread_mutex_lock(&mutex);
		while(!is_fan_on) pthread_cond_wait(&fan, &mutex);
		if(!thread_loop) break;
		fan_on();
		pthread_mutex_unlock(&mutex);
		delay(5000);
	}
	fan_off();
}

void *monitor(void *arg){
	int temp, i, humi, light, pre_sec;
	unsigned char adcChannel_light = 0;
	timer = time(NULL);
	t = localtime(&timer);
	pre_sec = t->tm_sec;
	while(thread_loop){
		pthread_mutex_lock(&mutex);
		while(count == MAX) pthread_cond_wait(&empty, &mutex);
		while(1){
			temp = read_temp_dat();
			if(temp) break;
		}
		light = read_mcp3208_adc(adcChannel_light);
		timer = time(NULL);
		t = localtime(&timer);
		printf("pre_sec : %d, t : %d\n", pre_sec, t->tm_sec);
		put(temp, light);
		if(light >= 2500) {
			is_led_on = 1; pthread_cond_signal(&led);
		}
		else{
			is_led_on = 0; led_off();
		}
		if(temp >=20) {
			is_fan_on = 1; pthread_cond_signal(&fan);
		}
		else{
			is_fan_on = 0; fan_off();
		}
		if((pre_sec + 10)%60 == t->tm_sec){
			pre_sec = t->tm_sec;
			is_send_data = 1; pthread_cond_signal(&fill);
		}
		pthread_mutex_unlock(&mutex);
		usleep(1);
	}
}
void *send_data(void *arg){
	int i;
	while(thread_loop){
		pthread_mutex_lock(&mutex);
		while(!is_send_data) pthread_cond_wait(&fill, &mutex);
		while(use_ptr != fill_ptr) get();
		is_send_data = 0;
		pthread_cond_signal(&empty);
		pthread_mutex_unlock(&mutex);
	}
}
void set_id(){
	if(mysql_query(connector, "SELECT count(*) FROM farm")){
		exit(EXIT_FAILURE);
	}
	MYSQL_RES *result = mysql_store_result(connector);
	if(result == NULL){
		exit(EXIT_FAILURE);
	}
	MYSQL_ROW row = mysql_fetch_row(result);
	for(int i=strlen(row[0])-1; i>=0; i--) id = id*10 + (row[0][i] - '0');
	id++;
}
int main(){
	signal(SIGINT, sig_handler);
	connector = mysql_init(NULL);
	if(!mysql_real_connect(connector, DBHOST, DBUSER, DBPASS, DBNAME, 3306, NULL, 0)){
		fprintf(stderr, "%s\n", mysql_error(connector));
		return 0;
	}
	printf("MYSQL open\n");
	if(wiringPiSetup() == -1)
		exit(EXIT_FAILURE);
	if(wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) == -1)
		exit(EXIT_FAILURE);
	set_id();
	led_init(); fan_init();
	printf("init\n");
	pthread_create(&p1, NULL, monitor,(void*) "monitor");
	pthread_create(&p2, NULL, send_data,(void*) "send_data");
	pthread_create(&p3, NULL, led_thread_func,(void*)"led");
	pthread_create(&p4, NULL, fan_thread_func, (void*)"fan");
	pthread_join(p1, NULL); printf("1\n");
	pthread_join(p2, NULL); printf("2\n");
	pthread_join(p3, NULL); printf("3\n");
	pthread_join(p4, NULL); printf("4\n");
	mysql_close(connector); printf("mysql close\n");
	return 0;	
}
