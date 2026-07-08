#include "buttons.h"
#include "app_state.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"


static const char *TAG = "BUTTONS";

static QueueHandle_t btn_event_queue = NULL;   // File interne pour signaler l'appuie d'un bouton



static void IRAM_ATTR btn_isr_handle(void *arg){

    uint32_t gpio_num = (uint32_t)arg;

    xQueueSendFromISR(btn_event_queue, &gpio_num, NULL);
}


void btn_init(){

    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << BTN_START) | (1ULL << BTN_STOP),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE   // interruption sur front descendant 
    };

    gpio_config(&io_conf);

    btn_event_queue = xQueueCreate(10, sizeof(uint32_t));  // creation d'une queue

    gpio_install_isr_service(0);

    //attacher le service isr aux gpios
    gpio_isr_handler_add(BTN_START, btn_isr_handle, (void*) BTN_START); 
    gpio_isr_handler_add(BTN_STOP, btn_isr_handle, (void*) BTN_STOP);

    ESP_LOGI(TAG, "boutons initialises");
}


void btn_task(void* pvParameters){

    uint32_t gpio_num;

    TickType_t last_press_time = 0;
    const TickType_t debounce = pdMS_TO_TICKS(200); // anti rebond pour les bouton

    while (1)
    {
        if(xQueueReceive(btn_event_queue, &gpio_num, portMAX_DELAY)){
            TickType_t now = xTaskGetTickCount();
            if ((now - last_press_time) < debounce)
                continue;   //rebond ignoré

            last_press_time = now;

            button_event_t event;

            if (gpio_num == BTN_START)
            {
                event = EVENT_BTN_START;
                ESP_LOGI(TAG, "Bouton start appuye");
            }else{
                event = EVENT_BTN_STOP;
                ESP_LOGI(TAG, "Bouton stop appuyé");
            }

            xQueueSend(g_button_queue, &event,0); 
        }
    }
}

