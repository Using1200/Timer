#pragma once

#include "driver/gpio.h"

#define BUZZER_GPIO GPIO_NUM_26


/**
 * @brief Initialise le buzzer 
 */
void buzzer_init(void);

/**
 * @brief Faire sonner le buzeer 
 */
void buzzer_on(void);

/**
 * @brief Arretes le buzzer
 */
void buzzer_off(void);

