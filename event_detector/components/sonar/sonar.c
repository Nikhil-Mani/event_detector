#include "sonar.h"
#include "driver/gpio.h"
#include "esp_attr.h"
#include "esp_log.h"
#include "esp_rom_sys.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TRIG GPIO_NUM_19
#define ECHO GPIO_NUM_18
#define SOUND_SPEED 0.034
volatile int64_t echo_start;
volatile int64_t echo_end;
bool echo_done;
static void gpio_isr_handler(void *arg) {
  int level = gpio_get_level(ECHO);
  if (level == 1) {
    echo_start = esp_timer_get_time();
  }
  if (level == 0) {
    echo_end = esp_timer_get_time();
    echo_done = true;
  }
}
void configure_gpios(void) {

  gpio_config_t echo_config = {};

  echo_config.pin_bit_mask = (1ULL << ECHO);
  echo_config.mode = GPIO_MODE_INPUT;
  echo_config.pull_up_en = GPIO_PULLUP_DISABLE;
  echo_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
  echo_config.intr_type = GPIO_INTR_ANYEDGE;
  gpio_config_t trig_config = {};
  trig_config.pin_bit_mask = (1ULL << TRIG);
  trig_config.mode = GPIO_MODE_OUTPUT;
  trig_config.pull_up_en = GPIO_PULLUP_DISABLE;
  trig_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
  trig_config.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&echo_config);
  gpio_config(&trig_config);
}

void configure_interrupt(void) {
  gpio_install_isr_service(0);
  gpio_isr_handler_add(ECHO, gpio_isr_handler, NULL);
  gpio_intr_enable(ECHO);
}

void init_gpio(void) {
  ESP_LOGI("example", "hi");
  configure_gpios();
  configure_interrupt();
}

esp_err_t sonar_run(float *distance_cm, int64_t *time) {
  int64_t duration;
  echo_done = false;
  // set trig low (reset)
  gpio_set_level(TRIG, 0);
  esp_rom_delay_us(1);
  // set high for 10 us
  gpio_set_level(TRIG, 1);
  esp_rom_delay_us(10);
  // set low again
  gpio_set_level(TRIG, 0);

  int64_t timeout = esp_timer_get_time() + 40000;
  while (!echo_done) {
    if (esp_timer_get_time() > timeout) {
      duration = -1;
    }
    vTaskDelay(1);
  }
  //  json not formatted!
  struct timeval cur_time;
  gettimeofday(&cur_time, NULL); // add in tz param
  int64_t t =
      ((uint64_t)cur_time.tv_sec * 1000000L) + (uint64_t)cur_time.tv_usec;
  duration = echo_end - echo_start;
  *time = t;
  *distance_cm = duration * SOUND_SPEED / 2;
  // ESP_LOGI("example", "Distance in centimeters is: %f", *distance_cm);
  return ESP_OK;
}
