#include "potentiometer.h"
#include "app_state.h"
#include "driver/adc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static char *TAG = "POTENTIOMETER";



void potentiometer_init(void){
    adc2_config_channel_atten(POT_ADC_CHANNEL, ADC_ATTEN_DB_12);
}


void potentiometer_task(void *pvParameters){
    while (1)
    {
        timer_info_t info;
        app_state_get(&info);

        if (info.state == TIMER_IDLE)
        {
            int pot_value;

            esp_err_t err = adc2_get_raw(POT_ADC_CHANNEL, ADC_WIDTH_BIT_12, &pot_value); // 0 --> 4095

            if (err == ESP_OK)
            {
                int seconds = (pot_value * TIMER_MAX_SECONDS) / 4095;
                //ESP_LOGI(TAG,"%04d", seconds);
                app_state_set_seconds(seconds);
            }else
            {
                ESP_LOGE(TAG, "Erruer recuperation valeur du potentiometre");
            }
            
        }

        vTaskDelay(pdMS_TO_TICKS(200)); // 5x en 1 seconde
        
    }
    
}