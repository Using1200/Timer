#include "display.h"
#include "app_state.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
 
static const char *TAG = "DISPLAY";


void display_init(void)
{

    ESP_LOGI(TAG, "Display initialise");
}

static const char *state_to_str(timer_state_t state){
    switch (state)
    {
        case TIMER_IDLE:        return "IDLE";
        case TIMER_RUNNING:     return "RUNNING";
        case TIMER_FINISHED:    return "FINISHED";
        default:                return ".....";
    }
}


void display_task(void *pvParameters){

    while (g_state_mutex == NULL) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    
    while (1)
    {
        timer_info_t info;
        app_state_get(&info);

        int minutes = info.seconds_remaining / 60;
        int seconds = info.seconds_remaining % 60;



        // completer avec ecran tft

        ESP_LOGI(TAG, "%02d:%02d   [%s]", minutes, seconds, state_to_str(info.state));
        
        vTaskDelay(pdMS_TO_TICKS(300));
    }
    
}