// Blink app
//
// Blinks the LEDs on Buckler

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"

#include "buckler.h"
#include "gpio.h"



int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");

  uint32_t *pOutset = (uint32_t*) 0x50000508;   //get these to point to right mem address
  uint32_t *pOutclear = (uint32_t*) 0x5000050C;
  uint32_t *pCnf = (uint32_t*) 0x50000700;

  uint32_t *pin23 = (uint32_t*) 0x5000075C;   //set LED pins to output
  *pin23 = 1;
  uint32_t *pin24 = (uint32_t*) 0x50000760;
  *pin24 = 1;
  uint32_t *pin25 = (uint32_t*) 0x50000764;
  *pin25 = 1;

  uint32_t *pin22 = (uint32_t*) 0x50000758;   //set SWITCH0 and BUTTON0 to input
 
  uint32_t *pin28 = (uint32_t*) 0x50000770;
 *pin22 = 0;
 *pin28 = 0;

  ////uint32_t *in_register = (uint32_t*) 0x50000510;
  ////*in_register = 0 << 22; //receive input from pin 22, on low (0)
  ////*in_register = 0 << 28; //receive input from pin 28, on low (0)
/*
  typedef struct {
    uint32_t out;
    uint32_t outset;
    uint32_t outclear;
    uint32_t in;
    uint32_t dir;
    uint32_t dirset;
    uint32_t dirclear;
  } pins;

  typedef struct {
    uint32_t cnf_pins[32];
  } cnf;

  cnf* cnf_instance;
  pins* pin_instance;

  uint32_t *cnf_instance.cnf_pins = (uint32_t*) 0x50000700;
  uint32_t *pin_instance.out = (uint32_t*) 0x50000504;

  cnf_instance->cnf_pins[22] = 0;
  cnf_instance->cnf_pins[28] = 0;
*/

  //uint32_t *cnf.cnf_pins = (uint32_t*) 0x50000700;
  //uint32_t *pins.out = (uint32_t*) 0x50000504;

  uint32_t *pIn = (uint32_t*)0x50000510;
  uint32_t mask_22;
  uint32_t mask_28;

  //*cnf.cnf_pins = 0 << 22;
  //*cnf.cnf_pins = 0 << 28;

  // loop forever
  while (1) {

    //*pOutclear =  0x7<<23;
    //*pPins = 0x7<<23;


    nrf_delay_ms(1000);
    
    //*pOutset =  0x7<<23;
    //*pPins = 0x7<<23;

  mask_22 = (1 << 22) & *pIn; //gets the value
  mask_28 = (1 << 28) & *pIn;

  if (mask_22 != 0 && mask_28 == 0)
  {
    *pOutclear =  0x7<<23;
  }
  else if (mask_22 == 0 && mask_28 != 0)
  {
    *pOutclear =  0x7<<23;
  }
  else
  {
    *pOutset =  0x7<<23;
  }

    printf("Button0: %x, SWITCH0: %x\n", mask_22, mask_28); //show values changing

    nrf_delay_ms(500);
  }
}

