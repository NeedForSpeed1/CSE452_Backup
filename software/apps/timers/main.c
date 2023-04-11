

//** Code: Hard Timers (Lab 6)
//** Author: Umer Huzaifa
//** Date: 12/30/2021
//** Comments: Keeping a separate app for the hardware timers and counters


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

void timer_init(){
	// fill in this function to initialize a timer of your choice

  //---------------TIMER SET UP-------------------// 

  NRF_TIMER4->PRESCALER = 0x09;        //set to highest possible to slow down clock 

  NRF_TIMER4->MODE = 0x00;             //set to timer mode

  NRF_TIMER4->BITMODE = 0x02;          //set to 32 bits 

  NRF_TIMER4->TASKS_CLEAR = 0x01;

  NRF_TIMER4->TASKS_START = 0x01;

  //---------------COUNTER SET UP-------------------// 
  NRF_TIMER3->MODE = 0x01;            //set to counter mode

  NRF_TIMER3->BITMODE = 0x00;         //set to 16 bits

  NRF_TIMER3->TASKS_START = 0x01;


  //f_time = 16 MHz / 2^9, which is 31250 KHz
  //time period is 3 seconds
  //val = time period * f_time - 1, which is 93749
  NRF_TIMER4->CC[0] = 93749;  

}

uint32_t read_timer(){

	// fill in this function for reading the timer value on calling this function
  NRF_TIMER4->TASKS_CAPTURE[1] = 0x01;
  uint32_t value_timer = NRF_TIMER4->CC[1];
  return value_timer;
}

int read_counter(){
  NRF_TIMER3->TASKS_CAPTURE[1] = 0x01;
  int value_counter = NRF_TIMER3->CC[1];
  return value_counter;
}


void TIMER4_IRQHandler(void) {
    NRF_TIMER4->EVENTS_COMPARE[0] = 0;

    NRF_TIMER4->TASKS_CLEAR = 0x01;
    
    printf("Button Interrupt Received\n");
    NRF_TIMER3->TASKS_COUNT = 0x01;           //increment counter by 1

    printf("Count is now: %d and Timer is: %d\n", read_counter(), read_timer());
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


  NRF_TIMER4->INTENSET = 0x1 << 16;   //enable interrupt for event[0], config[0] is at 1st bit of this register
  NVIC_EnableIRQ(TIMER4_IRQn);
  NVIC_SetPriority(TIMER4_IRQn, 0);
  
  timer_init();  

  //nrf_delay_ms(3000);

  // loop forever
  while (1) {

    printf("Looping, Timer Value: %d\n", read_timer());


    nrf_delay_ms (1000);

   // __WFI ();

  }
}

