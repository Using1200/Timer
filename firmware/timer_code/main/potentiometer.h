#pragma once


#define POT_ADC_CHANNEL ADC2_CHANNEL_8
#define TIMER_MAX_SECONDS 300  // 5minutes

void potentiometer_init(void);
void potentiometer_task(void *pvParameters);