// Blink app
//
// Blinks the LEDs on Buckler

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "app_timer.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_gpio.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"
#include "software_interrupt.h"

#include "buckler.h"




void SWI1_EGU1_IRQHandler(void) {
    NRF_EGU1->EVENTS_TRIGGERED[0] = 0;
    //software_interrupt_init();      //software interrupts
    //software_interrupt_generate();
    printf("Software interrupt is working\n");
}


void GPIOTE_IRQHandler(void) {
    
  if (NRF_GPIOTE->EVENTS_IN[1] != 0)
  {  
    NRF_GPIOTE->EVENTS_IN[1] = 0;
    printf("Switch Interrupt Received\n");
    nrf_delay_ms(500);
    nrf_delay_ms(500);
    nrf_gpio_pin_set(24);   //pulsing LED
    nrf_delay_ms(500);
    nrf_gpio_pin_clear(24);
  }
  else
  {
    NRF_GPIOTE->EVENTS_IN[0] = 0;
    printf("Button0 Interrupt Received\n");
    nrf_delay_ms(500);
    nrf_gpio_pin_set(23);   //pulsing LED
    nrf_delay_ms(500);
    nrf_gpio_pin_clear(23);
  }
}

/*
nrf_gpio_cfg_input(pin_nnumb, NRF_GPIO_PIN_NOPULL);
*/


int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");

  //need to set all relevant values at once or else it clears out if done by 
  //incremental bit shifts
  NRF_GPIOTE->CONFIG[0] = 0x21C01;
  NRF_GPIOTE->CONFIG[1] = 0x21601;

NRF_GPIOTE->INTENSET = 0x3;   //enable interrupt for event[0], config[0] is at 1st bit of this register
//was 0x1 for just event 0, but added in for event 1 now

NVIC_EnableIRQ(GPIOTE_IRQn); 
NVIC_SetPriority(GPIOTE_IRQn, 0);

nrf_gpio_cfg_output(23); //1st led at pin 23 set to output
nrf_gpio_cfg_output(24); //2nd led at pin 24 set to output

//software_interrupt_init();


//NVIC_EnableIRQ(SWI1_EGU1_IRQn); 
//NVIC_SetPriority(SWI1_EGU1_IRQn, 2);


  // loop forever
  while (1) {
    printf("Looping\n");
    //nrf_delay_ms(1000);

    __WFI ();

    //software_interrupt_generate();

    //__WFI ();
  }
}

