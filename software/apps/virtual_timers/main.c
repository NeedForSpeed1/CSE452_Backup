// Virtual timers
//
// Uses a single hardware timer to create an unlimited supply of virtual
//  software timers

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "app_timer.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"

#include "buckler.h"
#include "virtual_timer.h"

void led0_toggle() {
    nrf_gpio_pin_toggle(BUCKLER_LED0);
}

void led1_toggle() {
    nrf_gpio_pin_toggle(BUCKLER_LED1);
}

void led2_toggle() {
    nrf_gpio_pin_toggle(BUCKLER_LED2);
}

void timer_init(){
    // fill in this function to initialize a timer of your choice

  //---------------TIMER SET UP-------------------// 

  NRF_TIMER4->PRESCALER = 0x04;        //set to highest possible to slow down clock 

  NRF_TIMER4->MODE = 0x00;             //set to timer mode

  NRF_TIMER4->BITMODE = 0x02;          //set to 32 bits 

  NRF_TIMER4->TASKS_CLEAR = 0x01;

  NRF_TIMER4->TASKS_START = 0x01;

  //NRF_TIMER4->CC[0] = 0;      //set up in timer_start() in virtual_timers.c

}

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Board initialized!\n");

  // You can use the NRF GPIO library to test your timers
  nrf_gpio_pin_dir_set(BUCKLER_LED0, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(BUCKLER_LED1, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(BUCKLER_LED2, NRF_GPIO_PIN_DIR_OUTPUT);

  // Don't forget to initialize your timer library
  virtual_timer_init();
  nrf_delay_ms(3000);

  // Setup some timers and see what happens
  //virtual_timer_start_repeated(1000000, led0_toggle);
  //virtual_timer_start_repeated(2000000, led1_toggle);
  
  //make_five_timers();
  timer_init();
  virtual_timer_start(3000000, led0_toggle());
  virtual_timer_start(5000000, led1_toggle());
  virtual_timer_start(7000000, led2_toggle());

  
  NVIC_EnableIRQ(TIMER4_IRQn);          //enable and set priority
  NVIC_SetPriority(TIMER4_IRQn, 0);


  // loop forever
  while (1) {
    nrf_delay_ms(1000);
  }
}

