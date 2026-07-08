#include "app_state.h"
#include "buttons.h"
#include "buzzer.h"
#include "display.h"
#include "potentiometer.h"
#include "timer_logic.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    // 1. Initialiser l'etat partage (mutex + queue)
    app_state_init();

    // 2. Initialiser les peripheriques
    btn_init();
    buzzer_init();
    display_init();
    potentiometer_init();

    // 3. Creer les taches (nom, taille pile, param, priorite, handle)
    xTaskCreate(btn_task,       "buttons_task",       2048, NULL, 10, NULL);
    xTaskCreate(timer_logic_task,   "timer_logic_task",   3072, NULL, 8,  NULL);
    xTaskCreate(display_task,       "display_task",       3072, NULL, 4,  NULL);
    xTaskCreate(potentiometer_task, "potentiometer_task", 2048, NULL, 4,  NULL);
}