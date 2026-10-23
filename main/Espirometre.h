#ifndef ESPIROMETRE
#define ESPIROMETRE

#include "freertos/idf_additions.h"

// Definitions of I2C
#define I2C_PORT_NUM_0 0
#define SDA_IO 12
#define SCL_IO 14
#define SCL_FREQ 100000
#define DEV_ADDR 0xFF
#define DATA_LENGTH 11
// Definition of a Kilobyte
#define KB 1024

// Size of the whole block to process the data from the presure sensor
#define BLOCK_SIZE 1*KB

// Half of the whole block to process the data from the presure sensor
#define HBLOCK_SIZE KB/2

/**
 * @brief		Data needed from the enviroment
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

// Block that will contain the data of the sensor
volatile short I2C_read[BLOCK_SIZE] = {0};

/**
 * @brief		Estabishes a serial connection to the computer
 * 
 * @return		Returns the success code
 */
int connect_to_pc();

/**
 * @breif		Gets the enviromental variables to calibrate the models
 */
void get_env_var(TimerHandle_t xTimer);

/**
 * @brief		Configures the I2C to be able to read the sensor
 *
 * @return		Returns the success code
 */
int config_I2C();

/**
 * @brief		Reads the values from the sensor and stores it in the buffer
 */
void read_I2C(void *args);

/**
 * @breif		Process the data reicived from the sensor
 *
 * @return		Returns the success code
 */
int process_data();

/**
 * @breif		Sends the processed data to the computer
 *
 * @return		Returns the success code
 */
int send_data();

#endif /*ESPIROMETRE*/
