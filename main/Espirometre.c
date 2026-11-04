// Personal libraries
#include "Espirometre.h"
#include "esp_timer.h"
#include "sensor.h"
#include "runtime.h"
#include <stdint.h>
#include <stdio.h>

// Global variables
char str_buff[16*KB] = {0};
volatile uint32_t *DMA_buff;
volatile mesure_t data[BLOCK_SIZE] = {0};
send_t send[HBLOCK_SIZE] = {0};
volatile uint16_t pos = 0;
volatile uint16_t block = 0;

#define S	0x3<<30
#define PM	0x3f<<24
#define PL	0xff<<16
#define TM	0xff<<8
#define TL	0x7<<5
#define X	0x1f

// TODO delete. Used for debuging

void print_binary(uint32_t value, int bits)
{
    for (int i = bits - 1; i >= 0; i--) {
        putchar((value & (1U << i)) ? '1' : '0');
    }
	putchar('\n');
}
// TODO delete. Used for debuging

void app_main(void)
{
	uint32_t aux;
	config_SPI();
	esp_timer_handle_t thdl;
	esp_timer_create_args_t tcfg = {
		.arg = NULL,
		.callback = read_SPI,
		.name = "Pressure",
		.skip_unhandled_events = 1
	};
	esp_timer_create(&tcfg, &thdl);
	esp_timer_start_periodic(thdl, 550);
	while(1){
		aux = *DMA_buff;
		//print_binary(aux, 32);
		printf("dt: %lld\t", dt);
		print_binary(aux, 32);
		/*
		for(aux = 0; aux < BLOCK_SIZE; aux++){
			sprintf(str_buff+13*aux, "%03lu,%08lx\n", data[aux].dt, data[aux].p);
		}
		printf("%s",str_buff);
		*/
	}
}
