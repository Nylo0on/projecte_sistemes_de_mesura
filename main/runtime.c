#include "runtime.h"
#include "sensor.h"
#include "Espirometre.h"

// Global variables
SemaphoreHandle_t smph = NULL;
TaskHandle_t env_handle, spi_handle;
void create_tasks(){
	xTaskCreatePinnedToCore(read_SPI, "SPI", 1*KB, NULL, 24, &spi_handle,1);
	xTaskCreate(get_env_var, "ENV", 1*KB, NULL, 10, &env_handle);
}

float get_pressure(uint16_t val){
	return ((float)(val-8192))/6554;
}

uint32_t process_data(){
	uint16_t i;
	for(i = 0; i < HBLOCK_SIZE; i++){
		send[i+block].pressure = data[i+block].p;
		send[i+block].time = data[i+block].dt;
		send[i+block].flow = data[i+block].p*(K1 + K2*data[i+block].p);
		send[i+block].humidity = env_var.hum;
		send[i+block].atm = env_var.press;
	}
	return ESP_OK;
}

uint32_t send_data(){
	return ESP_OK;
}
