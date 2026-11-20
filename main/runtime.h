#ifndef RUNTIME_H
#define RUNTIME_H

#include "freertos/idf_additions.h"
#include <stdint.h>
#include "Espirometre.h"

#define K1 0.0001
#define K2 0.0001
#define BUFF_SIZE (50*KB)

#define OFFSET_P 0
#define OFFSET_O 0x0
#define FS 1
#define CALIBRATED_RANGE 1

typedef struct {
	uint32_t time;
	float flow;
	float temperature;
	float humidity;
	float atm;
	float volume;
} send_t;

// Global variables
extern SemaphoreHandle_t smph;
extern TaskHandle_t env_handle;
extern char str_buff[];
extern send_t send[HBLOCK_SIZE];
extern int32_t bias;
/**
 * @brief		Create the tasks that will be running all the time
 */
void create_tasks();

/**
 * @breif		Process the data reicived from the sensor
 *
 * @return		Returns the success code
 */
uint32_t process_data();

/**
 * @breif		Sends the processed data to the computer
 *
 * @return		Returns the success code
 */
uint32_t send_data();


void get_bias();

/**
 * @brief		Gets the current flow on the device
 *
 * @param val	Measure from the sensor
 * @return		Flow in L/s
 */
float get_flow(int16_t val);

void calibrate();
#endif /* RUNTIME_H */
