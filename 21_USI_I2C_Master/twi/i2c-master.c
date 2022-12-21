/*
 * attiny-i2c-master: An implementation of I2C master for Attiny
 *                    microcontrollers, using USI.
 * Copyright (C) 2019 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the BSD License.
 * See the file LICENSE.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include "i2c-master.h"

// must be bigger than 4.7us and 4us
// https://www.analog.com/en/technical-articles/i2c-timing-definition-and-specification-guide-part-2.html
// https://www.analog.com/-/media/analog/en/landing-pages/technical-articles/i2c-timing-definition-and-specification-guide-part-2-/figure-8.jpg?la=en&w=900
#define __delay()  _delay_us(5.2)
#define __delay2() _delay_us(4.5)


void
i2c_master_init()
{
    PORT_I2C |= (1 << SCL_I2C) | (1 << SDA_I2C);
    DDR_I2C  |= (1 << SCL_I2C) | (1 << SDA_I2C);

    USIDR = 0xFF;
    USICR = (1 << USIWM1) | (1 << USICS1) | (1 << USICLK);
    USISR = (1 << USISIF) | (1 << USIOIF) | (1 << USIPF) | (1 << USIDC);
}


static uint8_t
xfer(uint8_t data, bool bit)
{
    if (bit) {
        USISR = (1 << USISIF) | (1 << USIOIF) | (1 << USIPF) | (1 << USIDC) |
            (0xE << USICNT0);
    }
    else {
        USISR = (1 << USISIF) | (1 << USIOIF) | (1 << USIPF) | (1 << USIDC);
    }
    USIDR = data;

    do {
        __delay();

        USICR = (1 << USIWM1) | (1 << USICS1) | (1 << USICLK) | (1 << USITC);
        while ((PIN_I2C & (1 << SCL_I2C)) == 0);
        __delay2();

        USICR = (1 << USIWM1) | (1 << USICS1) | (1 << USICLK) | (1 << USITC);
        while ((PIN_I2C & (1 << SCL_I2C)) != 0);
    }
    while((USISR & (1 << USIOIF)) == 0);

    __delay();
    uint8_t rv = USIDR;
    USIDR = 0xFF;
    DDR_I2C |= (1 << SDA_I2C);

    return rv;
}


_I2C_LL bool
i2c_master_start_condition(void)
{
    PORT_I2C |= (1 << SCL_I2C);
    while ((PIN_I2C & (1 << SCL_I2C)) == 0);
    __delay();
    PORT_I2C &= ~(1 << SDA_I2C);
    __delay2();
    PORT_I2C &= ~(1 << SCL_I2C);
    while ((PIN_I2C & (1 << SCL_I2C)) != 0);
    PORT_I2C |= (1 << SDA_I2C);
    return USISR & (1 << USISIF);
}


_I2C_LL bool
i2c_master_stop_condition(void)
{
    PORT_I2C &= ~(1 << SDA_I2C);
    PORT_I2C |= (1 << SCL_I2C);
    while ((PIN_I2C & (1 << SCL_I2C)) == 0);
    __delay2();
    PORT_I2C |= (1 << SDA_I2C);
    __delay();
    return USISR & (1 << USIPF);
}


_I2C_LL bool
i2c_master_write_byte(uint8_t data)
{
    PORT_I2C &= ~(1 << SCL_I2C);
    while ((PIN_I2C & (1 << SCL_I2C)) != 0);
    xfer(data, false);
    DDR_I2C &= ~(1 << SDA_I2C);
    return (xfer(0xff, true) & (1 << 0)) == 0;
}


_I2C_LL bool
i2c_master_read_byte(uint8_t *data, bool last)
{
    DDR_I2C &= ~(1 << SDA_I2C);
    *data = xfer(0xff, false);
    return xfer(last ? 0xff : 0x00, true);
}


#ifndef I2C_LOW_LEVEL_API

bool
i2c_master_write(uint8_t slave_address, uint8_t reg_address, uint8_t *data, size_t data_len)
{
    if (!i2c_master_start_condition()) {
        return false;
    }

    if (!i2c_master_write_byte(slave_address << 1)) {
        return false;
    }

    if (!i2c_master_write_byte(reg_address)) {
        return false;
    }

    for (size_t i = 0; i < data_len; i++) {
        if (!i2c_master_write_byte(data[i])) {
            return false;
        }
    }

    return i2c_master_stop_condition();
}


bool
i2c_master_read(uint8_t slave_address, uint8_t reg_address, uint8_t *data, size_t data_len)
{
    if (!i2c_master_write(slave_address, reg_address, NULL, 0)) {
        return false;
    }

    if (!i2c_master_start_condition()) {
        return false;
    }

    if (!i2c_master_write_byte((slave_address << 1) | (1 << 0))) {
        return false;
    }

    if (data != NULL) {
        for (size_t i = 0; i < data_len; i++) {
            if (!i2c_master_read_byte(&(data[i]), data_len - i == 1)) {
                return false;
            }
        }
    }

    return i2c_master_stop_condition();
}

#endif
