/*
#include "gpio.h"

// Inputs: 
//  gpio_num - gpio number 0-31
//  dir - gpio direction (INPUT, OUTPUT)

uint32_t *pIn;    //need to make accessabile to all methods in this class
uint32_t pin_arg;

void gpio_config(uint8_t gpio_num, gpio_direction_t dir) {

  uint32_t *pOutset = (uint32_t*) 0x50000508;   //get these to point to right mem address
  uint32_t *pOutclear = (uint32_t*) 0x5000050C;
  uint32_t *pCnf = (uint32_t*) 0x50000700;

  *pIn = (uint32_t*)0x50000510;  //get input ready for setting later
                           

  //add pin num multiplied by 4 (bc that's the ptr size) to pCnf location
  //to access where that pin is located in memory for the board

  pin_arg = (uint32_t)gpio_num;

  uint32_t moved_spaces = 4 * gpio_num;

  *selected_pin = (uint_32_t*) pCnf + moved_spaces;

  if (dir == 0)
  {
    *selected_pin = 0;
  }
  else if (dir == 1)
  {
    *selected_pin = 1;
  }
  else
  {
    printf("Invalid input for pin's direction\n");
  }

}

// Set gpio_num high
// Inputs: 
//  gpio_num - gpio number 0-31
void gpio_set(uint8_t gpio_num) {

    *pIn = (1 << pin_arg); //sets the value to 1 (which is high for the pin)

}

// Set gpio_num low
// Inputs: 
//  gpio_num - gpio number 0-31
void gpio_clear(uint8_t gpio_num) {

    *pIn = (0 << pin_arg); //sets the value to 0 (which is low for the pin)

}

// Inputs: 
//  gpio_num - gpio number 0-31
bool gpio_read(uint8_t gpio_num) {
    // should return pin state

    uint32_t pin_state = *pIn << pin_arg;
    if (pin_state == 1)
    {
        return true;
    }
    return false;
}
*/

