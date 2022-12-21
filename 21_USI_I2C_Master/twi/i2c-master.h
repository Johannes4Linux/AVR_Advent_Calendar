/*
 * attiny-i2c-master: An implementation of I2C master for Attiny
 *                    microcontrollers, using USI.
 * Copyright (C) 2019 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the BSD License.
 * See the file LICENSE.
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny24A__) || \
    defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny44A__) || \
    defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny84A__)
#   define DDR_I2C DDRA
#   define PORT_I2C PORTA
#   define PIN_I2C PINA
#   define SDA_I2C 6
#   define SCL_I2C 4
#   define USI_STR_VECT_I2C USI_STR_vect
#   define USI_OVF_VECT_I2C USI_OVF_vect
#   define TIMSK_I2C TIMSK0
#   define TIMER_VECT_I2C TIM0_COMPA_vect
#elif defined(__AVR_ATtiny25__) || \
      defined(__AVR_ATtiny45__) || \
      defined(__AVR_ATtiny85__)
#   define DDR_I2C DDRB
#   define PORT_I2C PORTB
#   define PIN_I2C PINB
#   define SDA_I2C 0
#   define SCL_I2C 2
#   define USI_STR_VECT_I2C USI_START_vect
#   define USI_OVF_VECT_I2C USI_OVF_vect
#   define TIMSK_I2C TIMSK
#   define TIMER_VECT_I2C TIMER0_COMPA_vect
#elif defined(__AVR_ATtiny2313__) || defined(__AVR_ATtiny2313A__) || \
      defined(__AVR_ATtiny4313__)
#   define DDR_I2C DDRB
#   define PORT_I2C PORTB
#   define PIN_I2C PINB
#   define SDA_I2C 5
#   define SCL_I2C 7
#   define USI_STR_VECT_I2C USI_START_vect
#   define USI_OVF_VECT_I2C USI_OVERFLOW_vect
#   define TIMSK_I2C TIMSK
#   define TIMER_VECT_I2C TIMER0_COMPA_vect
#else
#   error "Unsupported Attiny microcontroller."
#endif

void i2c_master_init();

#ifdef I2C_LOW_LEVEL_API
#   define _I2C_LL
bool i2c_master_start_condition(void);
bool i2c_master_stop_condition(void);
bool i2c_master_write_byte(uint8_t data);
bool i2c_master_read_byte(uint8_t *data, bool last);
#else
#   define _I2C_LL static
bool i2c_master_write(uint8_t slave_address, uint8_t reg_address, uint8_t *data, size_t data_len);
bool i2c_master_read(uint8_t slave_address, uint8_t reg_address, uint8_t *data, size_t data_len);
#endif
