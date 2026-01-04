#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H
#include "esp_http_client.h"
#define LOGS_BEFORE_POST 5

struct httpRequest {
  esp_http_client_handle_t client;
  float sonar_data[LOGS_BEFORE_POST];
  int64_t time[LOGS_BEFORE_POST];
  TaskHandle_t parent_task;
};
esp_err_t _http_event_handler(esp_http_client_event_t *evt);
struct httpRequest *init_post(int logs_before_post);
void post_request(void *pvParameters);
char *format_json(float *distances, int64_t *times, int size);
#endif
