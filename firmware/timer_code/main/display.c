#include "display.h"
#include "app_state.h"
#include "tft_init.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
 
static const char *TAG = "DISPLAY";


void display_init(void)
{
    tft_controller_init(TFT_MODE_1);

    tft_fill_screen(TFT_BLACK);
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


void display_idle(timer_info_t info){

    int seconds  =  info.seconds_set % 60;
    int minutes = info.seconds_set / 60;
    static int last_minutes = -1;

    if(minutes != last_minutes){
        char time[32];
        snprintf(time, sizeof(time), "%02d : %02d", minutes, seconds);
        //tft_fill_screen(TFT_BLACK);
        ESP_LOGI(TAG, "%02d : %02d   [%s]", minutes, seconds, state_to_str(info.state));
        //tft_fill_rect_dma(100, 92, 80, 40, TFT_BLACK);
        draw_string(100, 94, time, TFT_WHITE, TFT_BLACK);
        last_minutes = minutes;
    }
}

void display_running(timer_info_t info){

    char time[32];
    //tft_fill_screen(TFT_BLACK);  
    int minutes = info.seconds_remaining / 60;
    int seconds = info.seconds_remaining % 60;

    
    ESP_LOGI(TAG, "%02d : %02d   [%s]", minutes, seconds, state_to_str(info.state));

    snprintf(time, sizeof(time), "%02d:%02d", minutes, seconds);
    //tft_fill_rect_dma(100, 92, 80, 40, TFT_BLACK);
    draw_string(100, 94, time, TFT_WHITE, TFT_BLACK);

}


void display_task(void *pvParameters){

    while (g_state_mutex == NULL) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    timer_state_t last_state = -1; 

    while (1)
    {
        timer_info_t info;
        app_state_get(&info);

        if(info.state != last_state){
            int text_color = TFT_WHITE;

            tft_fill_screen(TFT_BLACK); 

            tft_draw_string(10, 37, "minutes",2, TFT_WHITE, TFT_BLACK);
            tft_draw_string(170, 37, "seconds",2, TFT_WHITE, TFT_BLACK);

            if (info.state == TIMER_IDLE)
                text_color = TFT_BLUE;
            else if (info.state == TIMER_RUNNING)
                text_color = TFT_GREEN;
            else
                text_color = TFT_RED;
            
            tft_draw_string(170, 180, state_to_str(info.state), 2, text_color, TFT_BLACK);
        }
        
        int minutes = info.seconds_remaining / 60;
        int seconds = info.seconds_remaining % 60;

        //IDLE STATUS 
        if (info.state == TIMER_IDLE /*&& last_state != TIMER_IDLE */)
        {
            ESP_LOGI(TAG, "STATUT: IDLE");
            display_idle(info);
        }
        else if (info.state == TIMER_RUNNING)
        {
            ESP_LOGI(TAG, "%02d:%02d   [%s]", minutes, seconds, state_to_str(info.state));
            display_running(info);
        }
        else if (info.state == TIMER_FINISHED)
        {
            draw_string(100, 94, "00 : 00", TFT_WHITE, TFT_BLACK);
        }

        last_state = info.state;
        
        vTaskDelay(pdMS_TO_TICKS(250)); 

    }
}