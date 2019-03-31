#include "mcp3021.h"

MCP3021::MCP3021(I2C_HandleTypeDef* p_i2c_handle, uint8_t addr) {
    // Assign relevent variables
    p_i2c_ = p_i2c_handle;
    address = addr;
}

bool MCP3021::acknowledge() {
    // To check if the device exists w/o conversion,
    // send address w/ write, and wait for ACK
    uint32_t tmp_reg = p_i2c_->Instance->CR2;
    tmp_reg |= I2C_CR2_START;   // Enable START generation
    tmp_reg ^= I2C_CR2_RD_WRN;  // Write mode
    tmp_reg ^= I2C_CR2_NBYTES;  // 0 bytes transfer

    p_i2c_->Instance->CR2 = tmp_reg;

    // Kinda hacky but this shouldn't really be running
    // during critical times anyways
    HAL_Delay(1); // Wait 1ms to check ACK or NACK

    if (p_i2c_->Instance->CR2 & I2C_CR2_NACK) {
        // Received NACK, device did not acknowledge.
        return false;
    } else {
        return true;
    }
}

/* This could definitely be improved, more error handling could be done,
 * right now it can't handle stuff already being in the RXDR,
 * but it's good enough for now.
 */
int16_t MCP3021::single_conversion() {
    /* MCP3021 I2C transfer description:
     * 
     * START | ADDR[7] R/W | 0 0 0 0 D9 D8 D7 D6 | D5 D4 D3 D2 D1 D0 X X STOP |
     */
    uint16_t conversion_value;

    uint32_t tmp_reg = p_i2c_->Instance->CR2;
    tmp_reg |= I2C_CR2_START;   // Enable START generation
    tmp_reg |= I2C_CR2_RD_WRN;  // Read mode
    // We are transfering two bytes
    tmp_reg &= (0xFFFFFFFF &  (2 << I2C_CR2_NBYTES_Pos));

    p_i2c_->Instance->CR2 = tmp_reg; // Write config

    // Wait for RXNE flag
    while ((p_i2c_->Instance->ISR & I2C_ISR_RXNE) == 0);

    conversion_value = p_i2c_->Instance->RXDR << 6; // top four bits

    // Wait again
    while ((p_i2c_->Instance->ISR & I2C_ISR_RXNE) == 0);

    conversion_value |= p_i2c_->Instance->RXDR >> 2; // bottom 6 bits

    return conversion_value;
}