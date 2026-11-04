#ifndef ESPIROMETRE
#define ESPIROMETRE

#include "sensor.h"
#include <sys/types.h>

//Definition of calibration variables
#define K1 1
#define K2 1

// Definition of a KiloHertz
#define kHz 1000
// Definition of a Kilobyte
#define KB 1024

// Size of the whole block to process the data from the presure sensor
#define BLOCK_SIZE 1*KB

// Half of the whole block to process the data from the presure sensor
#define HBLOCK_SIZE KB/2

typedef struct {
	float pressure;
	float atm;
	float flow;
	float temperature;
	float humidity;
	uint32_t time;
} send_t;

// Global variables
extern char str_buff[16*KB];
extern volatile uint32_t *DMA_buff;
extern volatile mesure_t data[BLOCK_SIZE];
extern send_t send[HBLOCK_SIZE];
extern volatile uint16_t pos;
extern volatile uint16_t block;

#endif /*ESPIROMETRE*/
