#include "runtime.h"
#include "sensor.h"
#include "Espirometre.h"
#include "esp_timer.h"
#include <stdint.h>
#include <sys/types.h>

// Global variables
SemaphoreHandle_t smph = NULL;
TaskHandle_t env_handle, spi_handle;
char str_buff[BUFF_SIZE] = {0};
send_t send[HBLOCK_SIZE] = {0};
int32_t bias = 0, variance = 0;

void create_tasks(){
	//xTaskCreate(get_env_var, "ENV", 1*KB, NULL, 10, &env_handle);
	esp_timer_handle_t thdl;
	esp_timer_create_args_t tcfg = {
		.arg = NULL,
		.callback = read_SPI,
		.name = "Pressure",
		.skip_unhandled_events = 1
	};
	esp_timer_create(&tcfg, &thdl);
	esp_timer_start_periodic(thdl, 500);
}

uint32_t process_data(){
	uint16_t i;
	for(i = 0; i < HBLOCK_SIZE; i++){
		send[i].time = data[i+block].dt;
		send[i].flow = get_flow(((((int32_t)data[i+block].data)>>16)&0x3fff)-bias);
		send[i].temperature = env_var.temp;
		send[i].humidity = env_var.hum;
		send[i].atm = env_var.press;
		send[i].volume = send[i].flow * send[i].time/1000000 + (i==0?send[HBLOCK_SIZE-1]:send[i-1]).volume;
	}
	return ESP_OK;
}

uint32_t send_data(){
	uint32_t i, strlen=0;
	for(i = 0; i < HBLOCK_SIZE; i++){
		strlen += sprintf(str_buff+strlen, "%03lu,%+07.3f,%04.1f,%04.1f,%+08.3f\n",
				(uint32_t) send[i].time,
				send[i].flow,
				send[i].temperature,
				send[i].humidity,
				send[i].volume);
	}
	printf("%s",str_buff);
	return ESP_OK;
}

void get_bias(){
	uint32_t i, aux = 0;
	for(i = 0; i < HBLOCK_SIZE; i++){
		aux += (data[i+block].data>>16)&0x3fff;
	}
	bias = aux/HBLOCK_SIZE + 0.5;
}

void get_variance(){
	uint32_t i, aux = 0;
	for(i = 0; i < HBLOCK_SIZE; i++){
		aux+= (((data[i+block].data>>16)&0x3fff)-bias);
	}
	variance = aux/(HBLOCK_SIZE-1);
}

float get_flow(int16_t val){
	return val*(K1 + K2*(val<0?-val:val));
}

void calibrate(){
	int32_t i, sum = 0;
	for(i = 0; i < 120; i++){
		while(rdy==0);
		rdy = 0;
		get_bias();
		sum += bias;
	}
	bias = sum/120;
	get_variance();
}
