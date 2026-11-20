// Personal libraries
#include "Espirometre.h"
#include "esp_task_wdt.h"
#include "sensor.h"
#include "runtime.h"
#include "esp_log.h"

void app_main(void)
{
	config_SPI();
	create_tasks();
	esp_task_wdt_deinit();
	calibrate();
	ESP_LOGI("CALIBRATION","Bias = %ld\n", bias);
	vTaskDelay(pdMS_TO_TICKS(1500));
	while(1){
		while(rdy == 0){}
		rdy = 0;
		process_data();
		send_data();
	}
}
