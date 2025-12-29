#include "driver/gpio.h"
#include "esp_intr_types.h"
#include "esp_log.h"
#include "esp_rom_sys.h" // for ets_delay_us
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "http-client.h"
#include "sdkconfig.h"
#include "sonar.h"
#include "wifi.h"
#include <stdint.h>
#include <stdio.h>

esp_http_client_handle_t init() {
  init_gpio();
  init_wifi();
  return init_post();
}

void sensor_read(void *pvParameter) {
  for (int i = 0; i < 30; i++) {
    float distance_cm;
    sonar_run(&distance_cm);
    // add function that does the erro cehcking thing here
    char char_array[8];
    sprintf(char_array, "%.3f", distance_cm);
  }
}

void app_main(void) {

  esp_http_client_handle_t client = init();
  char *post_data = "shit here";
  struct httpRequest *req = malloc(sizeof(struct httpRequest));
  req->client = &client;
  req->post_data = post_data;
  vTaskDelay(1);
  xTaskCreate(sensor_read, "sensor", 4096, NULL, 1, NULL);
  // also want this task, not sure how to format this but wtv

  xTaskCreate(post_request, "post", 4096, req, 1,
              NULL); // make sure to remove the return type and itll compile
  // int64_t duration;
  // esp_err_t ret;
  // for (int i = 0; i < 300; i++) {
  //   ret = sonar_run(&duration);
  // }
}
