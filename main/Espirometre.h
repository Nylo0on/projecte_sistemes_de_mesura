#ifndef ESPIROMETRE
#define ESPIROMETRE

// Size of the whole block to process the data from the presure sensor
#define BLOCK_SIZE 1024

// Half of the whole block to process the data from the presure sensor
#define HBLOCK_SIZE 512

// ADC frequency of the pressure sensor [Hz]
#define ADC_FREQ 20000

/* @brief		Data needed from the enviroment
 *
 * @var	temp		Temperature [K]
 * @var press		Pressure [Pa]
 * @var hum		Humidity [%]
 */
typedef volatile struct {
	float temp;
	float press;
	float hum;
} env_var_t;

// Block that will contain the data of the pressure's ADC
volatile float ADC_read[BLOCK_SIZE] = {0};

/* @brief		Calibration of the ADC channels to reduce errors
 *
 * @return		Returns the success code
 */
int calibrate_ADC();

/* @brief		Estabishes a serial connection to the computer
 * 
 * @return		Returns the success code
 */
int connect_to_pc();

/* @breif		Gets the enviromental variables to calibrate the models
 *
 * @return		Returns the success code
 */
int get_env_var(env_var_t *env);

/* @brief		Starts the pressure ADC in continous mode
 *
 * @return		Returns the success code
 */
int start_ADC();

/* @breif		Process the data reicived from the pressure ADC
 *
 * @return		Returns the success code
 */
int process_ADC();

/* @breif		Sends the processed data to the computer
 *
 * @return		Returns the success code
 */
int send_data();

#endif /*ESPIROMETRE*/
