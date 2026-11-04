#ifndef RUNTIME_H
#define RUNTIME_H

#include "freertos/idf_additions.h"
#include <stdint.h>

// Global variables
extern SemaphoreHandle_t smph;
extern TaskHandle_t env_handle, spi_handle;
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

/**
 * @brief		Funtion that translates the value reicived to it's pressure
 *
 * @param val	Value read from the SPI
 * @return		Pressure in Pa
 */
float get_pressure(uint16_t val);
#endif /* RUNTIME_H */
