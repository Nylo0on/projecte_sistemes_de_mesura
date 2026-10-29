// C libraries
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// FreeRTOS libraries
#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "freertos/timers.h"
#include "freertos/semphr.h"

// Driver libraries
#include "driver/spi_master.h"
#include "driver/spi_common.h"
#include "hal/spi_types.h"

// Other libraries
#include "esp_err.h"
#include "esp_intr_types.h"

// Personal libraries
#include "Espirometre.h"

// Global variables
spi_device_handle_t dev_handle;
env_var_t env_var = {0};
uint32_t read_temp = 0;
TaskHandle_t env_handle;

void app_main(void)
{
	config_SPI();
	create_tasks();
	while(1){
		vTaskDelay(pdMS_TO_TICKS(1000));
		process_data();
		send_data();
	}
}

void create_tasks(){
	xTaskCreate(get_env_var(), "ENV", 1*KB, NULL, 10, &env_handle);
}

TaskFunction_t get_env_var(){
	printf("Getting enviromental variables");
}

uint32_t config_SPI(){
	DMA_buff = (uint32_t *)spi_bus_dma_memory_alloc(SPI1_HOST, sizeof(uint32_t),0);
	if(DMA_buff == NULL)
		return !ESP_OK;
	spi_bus_config_t mstr_cfg = {
		.mosi_io_num = NOT_CONFIG,
		.miso_io_num = MISO_PIN,
		.sclk_io_num = SCLK_PIN,
		.quadwp_io_num = NOT_CONFIG,
		.quadhd_io_num = NOT_CONFIG,
		.data_io_default_level = 1,
		.flags = SPICOMMON_BUSFLAG_MASTER,
		.isr_cpu_id = ESP_INTR_CPU_AFFINITY_AUTO
	};
	spi_device_interface_config_t dev_interface = {
		.command_bits = 0,
		.address_bits = 0,
		.dummy_bits = 0,
		.mode = 0,
		.clock_source = SPI_CLK_SRC_DEFAULT,
		.duty_cycle_pos = 0,
		.cs_ena_pretrans = 0,
		.cs_ena_posttrans = 0,
		.clock_speed_hz = 1*MHz,
		.input_delay_ns = 0,
		.sample_point = SPI_SAMPLING_POINT_PHASE_0,
		.spics_io_num = CS_PIN,
		.flags = 0,
		.queue_size = 0,
		.pre_cb = before_SPI(),
		.post_cb = after_SPI()
	};
	ESP_ERROR_CHECK(spi_bus_initialize(SPI1_HOST, &mstr_cfg, SPI_DMA_CH_AUTO));
	ESP_ERROR_CHECK(spi_bus_add_device(SPI1_HOST, &dev_interface, &dev_handle));
	return ESP_OK;
}

void read_SPI(){
	spi_transaction_t presure_read = {
		.flags = 0,
		.cmd = 0,
		.addr = 0,
		.length = 16,
		.user = NULL,
		.tx_data = NULL,
		.rx_buffer = (void *) DMA_buff
	};
	spi_transaction_t temp_presure_read = {
		.flags = 0,
		.cmd = 0,
		.addr = 0,
		.length = 24,
		.user = NULL,
		.tx_data = NULL,
		.rx_buffer = (void *) DMA_buff
	};
}

transaction_cb_t before_SPI(){

}

transaction_cb_t after_SPI(){

}

uint32_t process_data(){
	return ESP_OK;
}

uint32_t send_data(){
	return ESP_OK;
}
