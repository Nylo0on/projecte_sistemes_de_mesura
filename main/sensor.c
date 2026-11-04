// Driver libraries
#include "driver/spi_master.h"
#include "driver/spi_common.h"
#include "hal/spi_types.h"
// Other libraries
#include "freertos/idf_additions.h"
#include "esp_log.h"
#include "esp_timer.h"

// Personal libraries
#include "Espirometre.h"
#include "sensor.h"
#include "runtime.h"
#include <stdint.h>
// Global variables
spi_device_handle_t dev_handle;
env_var_t env_var = {0};
uint32_t read_temp = 0;
uint64_t t0 = 0, t1 = 0, dt = 0;
uint32_t aux;

void get_env_var(void *param){
	while(1){
		printf("Getting enviromental variables\n");
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

uint32_t config_SPI(){
	// SPI bus configuration
	spi_bus_config_t mstr_cfg = {
		.mosi_io_num = NOT_CONFIG,
		.miso_io_num = MISO_PIN,
		.sclk_io_num = SCLK_PIN,
		.quadwp_io_num = NOT_CONFIG,
		.quadhd_io_num = NOT_CONFIG,
		.flags = SPICOMMON_BUSFLAG_MASTER,
		.isr_cpu_id = ESP_INTR_CPU_AFFINITY_AUTO
	};
	// Sensor configuration
	spi_device_interface_config_t dev_interface = {
		.command_bits = 0,
		.address_bits = 0,
		.dummy_bits = 0,
		.mode = 0,
		.clock_source = SPI_CLK_SRC_DEFAULT,
		.duty_cycle_pos = 0,
		.cs_ena_pretrans = 0,
		.cs_ena_posttrans = 0,
		.clock_speed_hz = 700*kHz,
		.input_delay_ns = 0,
		.sample_point = SPI_SAMPLING_POINT_PHASE_0,
		.spics_io_num = CS_PIN,
		.flags = 0,
		.queue_size = 1,
		.pre_cb = before_SPI,
		.post_cb = after_SPI
	};
	ESP_ERROR_CHECK(spi_bus_initialize(SPI3_HOST, &mstr_cfg, SPI_DMA_CH_AUTO));
	ESP_ERROR_CHECK(spi_bus_add_device(SPI3_HOST, &dev_interface, &dev_handle));

	// Init of the DMA
	DMA_buff = heap_caps_malloc(sizeof(uint32_t),MALLOC_CAP_DMA);
	if(DMA_buff == NULL){
		ESP_LOGE("DMA", "Failed to initialize");
		return !ESP_OK;
	}
	*DMA_buff = 0; 
	return ESP_OK;
}

void read_SPI(void *param){
	spi_transaction_t pressure_read = {
		.flags = 0,
		.cmd = 0,
		.addr = 0,
		.length = 16,
		.user = NULL,
		.rx_buffer = (void *) DMA_buff
	};
	spi_transaction_t temp_presure_read = {
		.flags = 0,
		.cmd = 0,
		.addr = 0,
		.length = 24,
		.user = NULL,
		.rx_buffer = (void *) DMA_buff
	};
	spi_transaction_t read_all = {
		.flags = 0,
		.cmd = 0,
		.addr = 0,
		.length = 32,
		.user = NULL,
		.rx_buffer = (void *) DMA_buff
	};
	spi_device_transmit(dev_handle, &read_all);
}

void before_SPI(spi_transaction_t *trans){
	t0=esp_timer_get_time();
	dt = t0-t1;
	t1=esp_timer_get_time();
}

void after_SPI(spi_transaction_t *trans){
	aux = *DMA_buff;
	data[pos] = (mesure_t) {*DMA_buff,dt};
	pos	= (pos+1)%BLOCK_SIZE;
	block = pos < HBLOCK_SIZE ? HBLOCK_SIZE:0; 
}
