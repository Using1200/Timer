#pragma once 

#include "driver/gpio.h"

#define BTN_START   GPIO_NUM_16
#define BTN_STOP    GPIO_NUM_17


/**
 * @brief Initialise les boutons stop et start
 *
 * Creation d'une queue interne
 *
 */
void btn_init();

/**
 * @brief Tache pour la Réception de ISR
 *
 * Configure le bus SPI2, ajoute le device TFT, et initialise
 * les pins DC, RES, BL en sortie avec leurs niveaux par défaut.
 *
 * @return
 *     - ESP_OK           Initialisation réussie
 *     - ESP_ERR_NO_MEM   Echec allocation DMA
 *     - ESP_FAIL         Erreur SPI
 */
void btn_task(void * pvParameters);

