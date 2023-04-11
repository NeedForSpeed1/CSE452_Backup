// Virtual timer implementation

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "nrf.h"

#include "virtual_timer.h"
#include "virtual_timer_linked_list.h"

uint32_t id_list = 0;

// This is the interrupt handler that fires on a compare event
void TIMER4_IRQHandler(void) {
  // This should always be the first line of the interrupt handler!
  // It clears the event so that it doesn't happen again
  NRF_TIMER4->EVENTS_COMPARE[0] = 0;

  // Place your interrupt handler code here
  __disable_irq()

  node_t* curr_node = list_get_first();

  while (curr_node != NULL)
  {
      (*(curr_node->cbFunc))();

      curr_node = curr_node->next;
  }

  checkTimers();

  __enable_irq()

}

void checkTimers(){
	// update CC[0] value by looking at the linked list
	// node after the current one, and update CC[0] using
	// the timer_value from this node

  uint32_t old_timer_value = list_get_first()->timer_value;  //remove old node
  list_remove_first();

  uint32_t new_timer_value = old_timer_value + read_timer(); //create and insert new node
  timer_start(new_timer_value, NULL, false);

  TIMER4->CC[0] = list_get_first()->timer_value;   //update cc val based on new first node
}


// Read the current value of the timer counter
uint32_t read_timer(void) {

  // Should return the value of the internal counter for TIMER4
  return 0;
}

// Initialize TIMER4 as a free running timer
// 1) Set to be a 32 bit timer
// 2) Set to count at 1MHz
// 3) Enable the timer peripheral interrupt (look carefully at the INTENSET register!)
// 4) Clear the timer
// 5) Start the timer
void virtual_timer_init(void) {
  // Place your timer initialization code here
}

// Start a timer. This function is called for both one-shot and repeated timers
// To start a timer:
// 1) Create a linked list node (This requires `malloc()`. Don't forget to free later)
// 2) Setup the linked list node with the correct information
//      - You will need to modify the `node_t` struct in "virtual_timer_linked_list.h"!
// 3) Place the node in the linked list
// 4) Setup the compare register so that the timer fires at the right time
// 5) Return a timer ID
//
// Your implementation will also have to take special precautions to make sure that
//  - You do not miss any timers
//  - You do not cause consistency issues in the linked list (hint: you may need the `__disable_irq()` and `__enable_irq()` functions).
//
// Follow the lab manual and start with simple cases first, building complexity and
// testing it over time.
static uint32_t timer_start(uint32_t microseconds, virtual_timer_callback_t cb, bool repeated) {

  // Return a unique timer ID. (hint: What is guaranteed unique about the timer you have created?)

  node_t* t_node = (node_t*)malloc(sizeof(node_t)); //create space for timer node
  t_node->timer_value = microseconds;
  t_node->period = timer_value + timer_value;
  t_node->ID = id_list;
  id_list++;

  list_insert_sorted(t_node); //insert in sorted order based on timer_value attribute

  TIMER4->CC[0] = list_get_first()->timer_value; //how does it know TIMER4 ??

  return id_list;
}

// You do not need to modify this function
// Instead, implement timer_start
uint32_t virtual_timer_start(uint32_t microseconds, virtual_timer_callback_t cb) {
  return timer_start(microseconds, cb, false);
}

// You do not need to modify this function
// Instead, implement timer_start
uint32_t virtual_timer_start_repeated(uint32_t microseconds, virtual_timer_callback_t cb) {
  return timer_start(microseconds, cb, true);
}

// Remove a timer by ID.
// Make sure you don't cause linked list consistency issues!
// Do not forget to free removed timers.
void virtual_timer_cancel(uint32_t timer_id) {
}



//make a call to make 5 timers based on specs from lab using timer_start() that was implemented
void make_five_timers()
{
  //5 timers: 8000000, 500000, 1000000, 8000000 and 7000000

  timer_start(1000000, NULL, false);   //timer 1
  
  timer_start(8000000, NULL, false);   //timer 2

  timer_start(8000000, NULL, false);   //timer 3

  timer_start(7000000, NULL, false);   //timer 4

  timer_start(500000, NULL, false);    //timer 5

  list_print();   //print the list to see verify it worked
}
