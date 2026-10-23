// C libraries
#include <stdio.h>
#include <stdlib.h>

// FreeRTOS libraries
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "freertos/timers.h"
#include "freertos/semphr.h"

// Driver libraries
#include "driver/i2c.h"
#include "driver/i2c_master.h"
#include "hal/i2c_types.h"
#include "soc/clk_tree_defs.h"

// Personal libraries
#include "Espirometre.h"

// Global variables
i2c_master_bus_handle_t bus_handle;
TaskHandle_t I2C_handle;
i2c_master_dev_handle_t dev_handle;

void app_main(void)
{
	config_I2C();
	xTimerCreate("ENV", pdMS_TO_TICKS(1000), pdTRUE, (void *const) 1, get_env_var);
	xTaskCreate(read_I2C, "I2C", 1*KB, NULL, portPRIVILEGE_BIT|2, &I2C_handle);
}

void get_env_var(TimerHandle_t xTimer);

int config_I2C(){
	i2c_master_bus_config_t conf = {
		.clk_source = I2C_CLK_SRC_DEFAULT,
		.i2c_port = I2C_PORT_NUM_0,
		.scl_io_num = SCL_IO,
		.sda_io_num = SDA_IO,
		.glitch_ignore_cnt = 7,
	};

	i2c_new_master_bus(&conf, &bus_handle);

	i2c_device_config_t dev_cfg = {
		.device_address = DEV_ADDR,
		.dev_addr_length = I2C_ADDR_BIT_LEN_7,
		.scl_speed_hz = SCL_FREQ,
	};
	i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle);
	return ESP_OK;
}

void read_I2C(void *args){
	short buff[2];
	while(1){
		i2c_master_receive(dev_handle, buff, DATA_LENGTH, -1);
	}
}

int process_data();

int send_data();
