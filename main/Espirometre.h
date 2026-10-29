#ifndef ESPIROMETRE
#define ESPIROMETRE

#include "driver/spi_master.h"
#include "freertos/idf_additions.h"
#include <sys/types.h>

// Definitions of the SPI
#define NOT_CONFIG -1
#define MISO_PIN 12
#define SCLK_PIN 14
#define CS_PIN 27
#define DEV_ADDR

// Definition of a MegaHertz
#define MHz 1000000
// Definition of a Kilobyte
#define KB 1024

// Size of the whole block to process the data from the presure sensor
#define BLOCK_SIZE 1*KB

// Half of the whole block to process the data from the presure sensor
#define HBLOCK_SIZE KB/2

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
	float p;
	uint32_t dt;
} mesure_t;

// Memory of the DMA
uint32_t *DMA_buff;
// Block that will contain the data of the sensor
mesure_t data[BLOCK_SIZE] = {0};
// Possition of the current read
volatile uint16_t pos = 0;
// Current block available
volatile uint8_t block = 0;


/**
 * @brief		Create the tasks that will be running all the time
 */
void create_tasks();

/**
 * @brief		Estabishes a serial connection to the computer
 * 
 * @return		Returns the success code
 */
uint32_t connect_to_pc();

/**
 * @breif		Gets the enviromental variables to calibrate the models
 */
TaskFunction_t get_env_var();

uint32_t config_SPI();

/**
 * @brief		Function that stores the data on the bus
 */
void read_SPI();

transaction_cb_t before_SPI();

transaction_cb_t after_SPI();
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

#endif /*ESPIROMETRE*/
