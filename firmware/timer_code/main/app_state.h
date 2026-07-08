#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"


//les differents etats du minuteur
typedef enum{
    TIMER_IDLE,            //en attente, réglage via le potentiometre
    TIMER_RUNNING,        //decompte en cours
    TIMER_FINISHED       //decompte terminé
} timer_state_t;



//état partagé entre les taches
typedef struct{
    int seconds_remaining;      // temps restant
    int seconds_set;            // temps réglé via le potentiometre
    timer_state_t state;
} timer_info_t;

//evenements envoyes pr les boutons 
typedef enum{
    EVENT_BTN_START,
    EVENT_BTN_STOP
} button_event_t;


//resources globales
extern timer_info_t g_timer_info;
extern SemaphoreHandle_t g_state_mutex;
extern QueueHandle_t g_button_queue;



/**
 * @brief Initialise les ressources partagees 
 *
 * mutex, queue et les valeurs par defaut
 */
void app_state_init(void);


/**
 * @brief Initialise les ressources partagees 
 *
 * mutex, queue et les valeurs par defaut
 * 
 * @param out  
 */
void app_state_get(timer_info_t *out);


/**
 * @brief Initialise les ressources partagees 
 *
 * mutex, queue et les valeurs par defaut
 * 
 * @param seconds 
 */
void app_state_set_seconds(int seconds);


/**
 * @brief Lance le minuteur 
 */
void app_state_start(void);


/**
 * @brief Arretes le minuteur 
 */
void app_state_stop(void);


/**
 * @brief decremente seconds_remaining de 1 
 */
void app_state_tick_decrement(void);
