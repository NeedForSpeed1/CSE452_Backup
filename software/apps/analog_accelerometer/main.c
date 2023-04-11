// Analog accelerometer app
//
// Reads data from the ADXL327 analog accelerometer

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <math.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"
#include "nrfx_gpiote.h"
#include "nrfx_saadc.h"

#include "buckler.h"


// ADC channels
#define X_CHANNEL 0
#define Y_CHANNEL 1
#define Z_CHANNEL 2

#define BIAS 1.455
#define SENSITIVITY 0.4074
#define PI 3.14159265

// callback for SAADC events
void saadc_callback (nrfx_saadc_evt_t const * p_event) {
  // don't care about adc callbacks
}

// sample a particular analog channel in blocking mode
nrf_saadc_value_t sample_value (uint8_t channel) {
  nrf_saadc_value_t val;
  ret_code_t error_code = nrfx_saadc_sample_convert(channel, &val);
  APP_ERROR_CHECK(error_code);
  return val;
}

int main (void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();

  // initialize analog to digital converter
  nrfx_saadc_config_t saadc_config = NRFX_SAADC_DEFAULT_CONFIG;
  saadc_config.resolution = NRF_SAADC_RESOLUTION_12BIT;
  error_code = nrfx_saadc_init(&saadc_config, saadc_callback);
  APP_ERROR_CHECK(error_code);

  // initialize analog inputs
  // configure with 0 as input pin for now
  nrf_saadc_channel_config_t channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(0);
  channel_config.gain = NRF_SAADC_GAIN1_6; // input gain of 1/6 Volts/Volt, multiply incoming signal by (1/6)
  channel_config.reference = NRF_SAADC_REFERENCE_INTERNAL; // 0.6 Volt reference, input after gain can be 0 to 0.6 Volts

  // specify input pin and initialize that ADC channel
  channel_config.pin_p = BUCKLER_ANALOG_ACCEL_X;
  error_code = nrfx_saadc_channel_init(X_CHANNEL, &channel_config);
  APP_ERROR_CHECK(error_code);

  // specify input pin and initialize that ADC channel
  channel_config.pin_p = BUCKLER_ANALOG_ACCEL_Y;
  error_code = nrfx_saadc_channel_init(Y_CHANNEL, &channel_config);
  APP_ERROR_CHECK(error_code);

  // specify input pin and initialize that ADC channel
  channel_config.pin_p = BUCKLER_ANALOG_ACCEL_Z;
  error_code = nrfx_saadc_channel_init(Z_CHANNEL, &channel_config);
  APP_ERROR_CHECK(error_code);

  // initialization complete
  printf("Buckler initialized!\n");

  float x_v;
  float y_v;
  float z_v;
  float lsb = 0.000878f;

  int i = 0;
  float angle = 0;

  double x_preArc;

  float radians = 180/ PI;

  double x_g;

  // loop forever
  while (1) {

    // sample analog inputs
    nrf_saadc_value_t x_val = sample_value(X_CHANNEL);
    //nrf_saadc_value_t y_val = sample_value(Y_CHANNEL);
    //nrf_saadc_value_t z_val = sample_value(Z_CHANNEL);

    x_v = (float)((int)x_val * lsb);
    //y_v = (float)((int)y_val * lsb);
    //z_v = (float)((int)z_val * lsb);

    x_preArc = (x_v - BIAS)/SENSITIVITY;

    //x_g =  (SENSITIVITY * x_v) + BIAS;

    angle = (float)(asin(x_preArc)) *  radians;

    //angle = (float)(asin(x_g)) *  radians;

    // display results
   // printf("x: %d, V: %f\ty: %d, V: %f\tz:%d, V: %f\n", x_val, x_v, y_val, y_v, z_val, z_v);
    printf("angle  %f,  i  %d\n",angle, i);
    // printf("x_g  %f,  i  %d\n",x_g, i);
    i+=1;
    nrf_delay_ms(500);
  }
}


