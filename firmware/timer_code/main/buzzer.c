#include "buzzer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


void buzzer_init(void){
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << BUZZER_GPIO),
        .mode = GPIO_MODE_OUTPUT,
    };

    gpio_config(&io_conf);
    gpio_set_level(BUZZER_GPIO, 0);
}


void buzzer_on(){
    gpio_set_level(BUZZER_GPIO, 1);
}

void buzzer_off(){
    gpio_set_level(BUZZER_GPIO, 0);
}