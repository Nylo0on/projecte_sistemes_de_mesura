idf_component_register(SRCS "Espirometre.c" "runtime.c" "sensor.c"
                    INCLUDE_DIRS "."
					REQUIRES driver esp_driver_spi esp_timer)
