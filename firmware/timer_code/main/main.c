#include "app_state.h"
#include "buttons.h"
#include "buzzer.h"
#include "display.h"
#include "potentiometer.h"
#include "timer_logic.h"
#include "tft_init.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    
    app_state_init();

    // 2. Init des peripheriques
    btn_init();
    buzzer_init();
    display_init();
    potentiometer_init();

    // 3. Creations des taches 
    xTaskCreate(btn_task,       "buttons_task",       2048, NULL, 10, NULL);
    xTaskCreate(timer_logic_task,   "timer_logic_task",   3072, NULL, 8,  NULL);
    xTaskCreate(display_task,       "display_task",       4096, NULL, 4,  NULL);
    xTaskCreate(potentiometer_task, "potentiometer_task", 2048, NULL, 4,  NULL);


    /*tft_controller_init(TFT_MODE_2);
    tft_set_mode(TFT_MODE_1);

    // test visuel
    tft_fill_screen(0xFFFF);
    //tft_fill_rect(0, 20, 60, 30, 0x07E0);

    tft_draw_string(50, 50, "Bonjour123",0x0000 , 0xFFFF);*/
}