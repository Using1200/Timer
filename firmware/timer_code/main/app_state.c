#include "app_state.h"
#include <string.h>


timer_info_t g_timer_info;
SemaphoreHandle_t g_state_mutex;
QueueHandle_t g_button_queue;

#define DEFAULT_SECONDS 60

void app_state_init(void){

    g_state_mutex = xSemaphoreCreateMutex();
    g_button_queue = xQueueCreate(10, sizeof(button_event_t));

    g_timer_info.seconds_remaining = DEFAULT_SECONDS;
    g_timer_info.seconds_set = DEFAULT_SECONDS;
    g_timer_info.state = TIMER_IDLE;
}


void app_state_get(timer_info_t *out){
    if (xSemaphoreTake(g_state_mutex, portMAX_DELAY) == pdTRUE)
    {
        *out = g_timer_info;
        xSemaphoreGive(g_state_mutex);
    }
}


void app_state_set_seconds(int seconds){
    if (xSemaphoreTake(g_state_mutex, portMAX_DELAY) == pdTRUE)
    {
        if (g_timer_info.state == TIMER_IDLE)
        {
            g_timer_info.seconds_remaining = seconds;
            g_timer_info.seconds_set = seconds;
        }

        xSemaphoreGive(g_state_mutex);
        
    }
    
}


void app_state_start(void){
    if (xSemaphoreTake(g_state_mutex, portMAX_DELAY))
    {
        if (g_timer_info.state != TIMER_RUNNING)
        {
            g_timer_info.seconds_remaining = g_timer_info.seconds_set;
            g_timer_info.state = TIMER_RUNNING;
        }
        xSemaphoreGive(g_state_mutex);
    }
}


void app_state_stop(void){
    if (xSemaphoreTake(g_state_mutex, portMAX_DELAY))
    {
        g_timer_info.state = TIMER_IDLE;
        g_timer_info.seconds_remaining = g_timer_info.seconds_set;

        xSemaphoreGive(g_state_mutex);
    }
}

void app_state_tick_decrement(void){
    if (xSemaphoreTake(g_state_mutex, portMAX_DELAY))
    {
        if (g_timer_info.state == TIMER_RUNNING)
        {
            if (g_timer_info.seconds_remaining > 0)
                g_timer_info.seconds_remaining--;

            if(g_timer_info.seconds_remaining == 0)
                g_timer_info.state = TIMER_FINISHED;
    
        }
        xSemaphoreGive(g_state_mutex);
    }
    
}