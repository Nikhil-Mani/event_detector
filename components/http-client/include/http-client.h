#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H
#include "esp_http_client.h"
esp_err_t _http_event_handler(esp_http_client_event_t *evt);
void init_http(void);
#endif
