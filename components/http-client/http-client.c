#include "http-client.h"
#include "esp_http_client.h"
#include <stdio.h>

#define MAX_HTTP_RECV_BUFFER 512
#define MAX_HTTP_OUTPUT_BUFFER 2048

char *endpoint = "https://example.com/";

esp_err_t _http_event_handler(esp_http_client_event_t *evt) {
  static char *output_buffer;
  static int output_len;

  switch (evt->event_id) {
  case HTTP_EVENT_ERROR:
    ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
    break;
  case HTTP_EVENT_ON_CONNECTED:
    ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
    break;
  case HTTP_EVENT_HEADER_SENT:
    ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
    break;
  case HTTP_EVENT_ON_HEADER:
    ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key,
             evt->header_value);
    break;
  case HTPP_EVENT_ON_DATA:
    if (output_len == 0 && evt->user_data) {
      memset(evt->user_data, 0, MAX_HTTP_OUTPUT_BUFFER);
    }

    if (!esp_http_client_is_chunked_response(evt->client)) {
      // If user_data buffer is configured, copy the response into the buffer
      int copy_len = 0;
      if (evt->user_data) {
        // The last byte in evt->user_data is kept for the NULL character in
        // case of out-of-bound access.
        copy_len = MIN(evt->data_len, (MAX_HTTP_OUTPUT_BUFFER - output_len));
        if (copy_len) {
          memcpy(evt->user_data + output_len, evt->data, copy_len);
        }
      } else {
        int content_len = esp_http_client_get_content_length(evt->client);
        if (output_buffer == NULL) {
          // We initialize output_buffer with 0 because it is used by strlen()
          // and similar functions therefore should be null terminated.
          output_buffer = (char *)calloc(content_len + 1, sizeof(char));
          output_len = 0;
          if (output_buffer == NULL) {
            ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
            return ESP_FAIL;
          }
        }
        copy_len = MIN(evt->data_len, (content_len - output_len));
        if (copy_len) {
          memcpy(output_buffer + output_len, evt->data, copy_len);
        }
      }
      output_len += copy_len;
    }
    break;
  }
}

void init_http(void) {

  char local_response_buffer[MAX_HTTP_OUTPUT_BUFFER + 1] = {0};
  esp_http_client_config_t config = {
      .host = &endpoint,
      .path = "/get",
      .query = "esp",
      .event_handler = _http_event_handler,
      .user_data =
          local_response_buffer, // Pass address of local buffer to get response
      .disable_auto_redirect = true,
  };
  ESP_LOGI(TAG, "HTTP request with url =>");
  esp_http_client_handle_t client = esp_http_client_init(&config);

  esp_err_t ret = esp_http_client_perform(client);

  if (ret == ESP_OK) {
    ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %" PRId64,
             esp_http_client_get_status_code(client),
             esp_http_client_get_content_length(client));
  } else {
    ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(ret));
  }
  ESP_LOG_BUFFER_HEX(TAG, local_response_buffer, strlen(local_response_buffer));
}
