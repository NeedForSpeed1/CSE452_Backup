// Blink app
//
// Blinks the LEDs on Buckler

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_gpiote.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"

#include "buckler.h"

// LED array
static uint8_t LEDS[3] = {BUCKLER_LED0, BUCKLER_LED1, BUCKLER_LED2};

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize GPIO driver
  if (!nrfx_gpiote_is_init()) {
    error_code = nrfx_gpiote_init();
  }
  APP_ERROR_CHECK(error_code);

  // configure leds
  // manually-controlled (simple) output, initially set
  nrfx_gpiote_out_config_t out_config = NRFX_GPIOTE_CONFIG_OUT_SIMPLE(true);
  for (int i=0; i<3; i++) {
    error_code = nrfx_gpiote_out_init(LEDS[i], &out_config);
    APP_ERROR_CHECK(error_code);
  }

  //create array of 3 ints for lab2, get address
  //int lab2[] = {0, 1, 2};


  // loop forever
  while (1) {
    for (int i=0; i<3; i++) {
      nrf_gpio_pin_toggle(LEDS[i]);
      nrf_delay_ms(500);
    }

    //melody of song for lab 2
    /*
    for (int j = 0; j < 4; j++){
      for (int i = 0; i < 3; i++){
       nrf_gpio_pin_toggle(LEDS[0]);
       nrf_delay_ms(250);
       nrf_gpio_pin_toggle(LEDS[0]);
       nrf_gpio_pin_toggle(LEDS[1]);
       nrf_delay_ms(250);
       nrf_gpio_pin_toggle(LEDS[1]);
      }
      nrf_gpio_pin_toggle(LEDS[2]);
      nrf_delay_ms(2500);
      nrf_gpio_pin_toggle(LEDS[2]);
    }
    */
    //bass for song for lab 2
    /*
    for (int k = 0; k < 7; k++) {
       nrf_gpio_pin_toggle(LEDS[0]);
       nrf_delay_ms(500);
       nrf_gpio_pin_toggle(LEDS[0]);
       nrf_gpio_pin_toggle(LEDS[1]);
       nrf_delay_ms(500);
       nrf_gpio_pin_toggle(LEDS[1]);

       nrf_gpio_pin_toggle(LEDS[0]);
       nrf_delay_ms(250);
       nrf_gpio_pin_toggle(LEDS[0]);
       nrf_gpio_pin_toggle(LEDS[2]);
       nrf_delay_ms(250);
       nrf_gpio_pin_toggle(LEDS[2]);
       nrf_gpio_pin_toggle(LEDS[0]);
       nrf_delay_ms(250);
       nrf_gpio_pin_toggle(LEDS[0]);
       nrf_gpio_pin_toggle(LEDS[2]);
       nrf_delay_ms(250);
       nrf_gpio_pin_toggle(LEDS[2]);
    }
    nrf_gpio_pin_toggle(LEDS[0]);
    nrf_gpio_pin_toggle(LEDS[1]);
    nrf_delay_ms(1000);
    nrf_gpio_pin_toggle(LEDS[0]);
    nrf_gpio_pin_toggle(LEDS[1]);
    nrf_gpio_pin_toggle(LEDS[2]);
    nrf_delay_ms(1000);
    nrf_gpio_pin_toggle(LEDS[2]);
    */
  }
}

