#include "timer_logic.h"
#include "app_state.h"
#include "buzzer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "TIMER_LOGIC";

void timer_logic_task(void *pvParameters){
    button_event_t event;
    TickType_t last_tick = xTaskGetTickCount();
    const TickType_t one_second = pdMS_TO_TICKS(1000);

    while (g_button_queue == NULL) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    
    while (1)
    {
        if (xQueueReceive(g_button_queue, &event, pdMS_TO_TICKS(50)) == pdTRUE)
        {
            if (event == EVENT_BTN_START)
            {
                app_state_start();
                buzzer_off();


                ESP_LOGI(TAG, "Minuteur demarré");

            }else if (event == EVENT_BTN_STOP)
            {
                app_state_stop();
                buzzer_off();

                ESP_LOGI(TAG, "Minuteur et buzzer arrete");
            }
        }

        if (xTaskGetTickCount() - last_tick >= one_second)
        {
            last_tick = xTaskGetTickCount();

            timer_info_t info;
            app_state_get(&info);

            if (info.state == TIMER_RUNNING)
            {
                app_state_tick_decrement();
                app_state_get(&info);

                if (info.state == TIMER_FINISHED)
                {
                    buzzer_on();

                    ESP_LOGI(TAG, "Minuteur termine et activation du buzzer");
                }   
            }   
        }   
    }  
}

