#ifndef MCP3021_H_
#define MCP3021_H_

/*
 * Library for MCP3021 10-bit I2C ADC from microchip.
 * 
 * The device is very simple to use, and is kind of non-standard
 * with how it does I2C, so I'm just going to assume I2C is already
 * initialized with the correct parameters (max 100kHz) and do all
 * the stuff in the library.
 * 
 * (Currently implemented for STM32F3)
 * 
 * Blame David Lenfesty for this code.
 */

#include "stm32f3xx.h"

class MCP3021 {
    public:
        MCP3021(I2C_HandleTypeDef* p_i2c_handle, uint8_t addr);

        // Check if device returns ACK on bus
        bool acknowledge();
        // Run a single conversion (blocking operation)
        // Returns -1 if error.
        int16_t single_conversion();

        // TODO multiple conversions support

    private:
        I2C_HandleTypeDef* p_i2c_;
        uint8_t address;
};


#endif