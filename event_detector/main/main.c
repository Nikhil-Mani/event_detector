#include "driver/gpio.h"
#include "esp_intr_types.h"
#include "esp_log.h"
#include "esp_rom_sys.h" // for ets_delay_us
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include <http-client.h>
#include <sonar.h>
#include <stdint.h>
#include <stdio.h>
#include <wifi.h>

void init() {
  init_gpio();
  init_wifi();
  init_http();
}

void app_main(void) {
  vTaskDelay(1);
  init();
  // int64_t duration;
  // esp_err_t ret;
  // for (int i = 0; i < 300; i++) {
  //   ret = sonar_run(&duration);
  // }
}
