/**
 * Copyright (c) 2015 - 2020, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/** @file
 * @defgroup pwm_example_main main.c
 * @{
 * @ingroup pwm_example
 *
 * @brief  PWM Example Application main file.
 *
 * This file contains the source code for a sample application using PWM.
 *
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "app_error.h"
#include "bsp.h"
#include "nrf_delay.h"
#include "app_pwm.h"\
#include "app_timer.h"


#define TIMER1 1
#define TIME_PERIOD 3000
#define TICKS 93749

// Create the instance "PWM1" using TIMER1.

APP_PWM_INSTANCE(PWM1, TIMER1); 


void timer_init(){

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


  NRF_TIMER4->CC[0] = TICKS; //bc time period set to 3000 ms for pwm
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

int getFreq()
{
    //frequency is 1/time_period
    int freq = 1/TIME_PERIOD;
    return freq;
}

int getDuty()
{
    //duty = t_active / time_period
    //wrapping the pwm lib's method in this to get int data type from it for recording in handler
    int curr_duty = (int)(app_pwm_channel_duty_get(PWM1, 1));

    return curr_duty;
}

void TIMER4_IRQHandler(void) {
    NRF_TIMER4->EVENTS_COMPARE[0] = 0;

    NRF_TIMER4->TASKS_CLEAR = 0x01;
    
    printf("Button Interrupt Received\n");
    NRF_TIMER3->TASKS_COUNT = 0x01;           //increment counter by 1

    printf("Count: %d, Frequency: %d, Duty Cycle: %d\n", read_counter(), getFreq(), getDuty());
}


int main(void)
{

    ret_code_t err_code;


    NRF_TIMER4->INTENSET = 0x1 << 16;   //enable interrupt for event[0], config[0] is at 1st bit of this register
    NVIC_EnableIRQ(TIMER4_IRQn);
    NVIC_SetPriority(TIMER4_IRQn, 0);
  
    timer_init();  

    /* Initialize PWM. */
    app_pwm_config_t config_pwm =  APP_PWM_DEFAULT_CONFIG_1CH(TIME_PERIOD, 11); //set to pin 11

    pwm_config.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_HIGH;

    //set up duty cycle (using channel 1, duty cycle is at 50%)
    app_pwm_channel_duty_set(PWM1, 1, 50); 


    /* Enable PWM. */
    app_pwm_enable(PWM1);

    uint32_t value;
    while (true)
    {

            nrf_delay_ms(1000);
        
    }

}


/** @} */

