#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H
#include "esp_http_client.h"

struct httpRequest {
  esp_http_client_handle_t *client;
  char *post_data;
};
esp_err_t _http_event_handler(esp_http_client_event_t *evt);
esp_http_client_handle_t init_post(void);
void post_request(void *pvParameters);
#endif
