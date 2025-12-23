#ifndef SONAR_H
#define SONAR_H

#include "esp_err.h"

void configure_gpios(void);
void configure_interrupt(void);
void init_gpio(void);
esp_err_t sonar_run(int64_t *duration);

#endif