#ifndef SENSOR_H
#define SENSOR_H

#include "driver/spi_master.h"

// Definitions of the SPI
#define NOT_CONFIG -1
#define MISO_PIN 19
#define SCLK_PIN 18
#define CS_PIN 5

/**
 * @brief			Data needed from the enviroment
 *
 * @var	temp		Temperature [K]
 * @var press		Pressure [Pa]
 * @var hum			Humidity [%]
 */
typedef volatile struct {
	float temp;
	float press;
	float hum;
} env_var_t;

/**
 * @brief			Data that will be sent to the PC
 *
 * @var p			Pressure of the sensor
 * @var t			Time respect the last mesure
 */
typedef volatile struct {
	uint32_t p;
	uint32_t dt;
} mesure_t;

// Global variables
extern spi_device_handle_t dev_handle;
extern env_var_t env_var;
extern uint32_t read_temp;
extern uint64_t t0, t1, dt;

/**
 * @breif			Gets the enviromental variables to calibrate the models
 */
void get_env_var(void *param);

/**
 * @brief			Configures the SPI bus, device and DMA
 *
 * @return			Status of the function: ESP_OK or !ESP_OK
 */
uint32_t config_SPI();

/**
 * @brief			Function that stores the data on the bus
 */
void read_SPI(void *param);

/**
 * @brief			Things to do before an SPI read
 *
 * @param trans		Compulsary parameter
 */
void before_SPI(spi_transaction_t *trans);

/**
 * @brief			Things to do after an SPI read
 *
 * @param trans		Compulsary parameter
 */
void after_SPI(spi_transaction_t *trans);

#endif /* SENSOR_H */
