#include "driver/gpio.h"
#include "esp_intr_types.h"
#include "esp_log.h"
#include "esp_rom_sys.h" // for ets_delay_us
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include <cstdio>
#include <http-client.h>
#include <sonar.h>
#include <stdint.h>
#include <stdio.h>
#include <wifi.h>

void init() {
  init_gpio();
  init_wifi();
}

esp_err_t sensor_post_task(void *pvParameter) {
  for (int i = 0; i < 20; i++) {
    float distance_cm;
    esp_err_t ret;
    ret = sonar_run(&distance_cm);
    char char_array[8];
    sprintf(char_array, "%.3f", distance_cm);
  }
  return ret;
}

void app_main(void) {
  vTaskDelay(1);
  init();
  init_post();
  xTaskCreate(sensor_post_task, "sensor", 4096, NULL, 1, NULL);
  // int64_t duration;
  // esp_err_t ret;
  // for (int i = 0; i < 300; i++) {
  //   ret = sonar_run(&duration);
  // }
}
