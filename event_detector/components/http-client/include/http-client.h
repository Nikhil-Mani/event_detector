#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H
#include "esp_http_client.h"

struct httpRequest {
  esp_http_client_handle_t client;
  char *post_data;
  int logs_before_post;
};
esp_err_t _http_event_handler(esp_http_client_event_t *evt);
struct httpRequest *init_post(int logs_before_post);
void post_request(void *pvParameters);
#endif
