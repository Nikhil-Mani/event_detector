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

static const char *TAG = "Main";
struct httpRequest *init() {
  init_gpio();
  init_wifi();
  init_time();
  return init_post(LOGS_BEFORE_POST);
}

void sensor_read(void *pvParameter) {
  struct httpRequest *req = init();
  float distances[LOGS_BEFORE_POST];
  int64_t times[LOGS_BEFORE_POST];
  TaskHandle_t t = xTaskGetCurrentTaskHandle();

  req->parent_task = t;
  for (int i = 1; i < 30; i++) {
    float distance;
    int64_t time;
    ESP_ERROR_CHECK(sonar_run(&distance, &time));
    ESP_LOGI(TAG, "%llu", time);
    distances[(i % LOGS_BEFORE_POST) - 1] = distance;
    times[(i % LOGS_BEFORE_POST) - 1] = time;
    if (i % LOGS_BEFORE_POST == 0) {
      memcpy(req->sonar_data, distances, LOGS_BEFORE_POST * sizeof(float));
      memcpy(req->times, times, LOGS_BEFORE_POST * sizeof(int64_t));
      // xTaskCreate(post_request, "post", 4096, req, 1, NULL);
      // ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    }
  }
  esp_http_client_cleanup(req->client);
  ESP_LOGI(TAG, "done");
  free(req);
  vTaskDelete(NULL);
}

void app_main(void) {
  vTaskDelay(1);
  xTaskCreate(sensor_read, "sensor", 4096, NULL, 1, NULL);
}
