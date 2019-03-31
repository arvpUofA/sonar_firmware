#include "i2c.h"

I2C::I2C() {

}

// Initialise I2C
I2C::I2C(uint8_t instance_num, uint32_t frequency, uint8_t own_address) {
    switch (instance_num) {
        case (1):
            __HAL_RCC_I2C1_CLK_ENABLE();
            hi2c.Instance = I2C1;
            break;
        case (2):
            __HAL_RCC_I2C2_CLK_ENABLE();
            hi2c.Instance = I2C2;
            break;
        default:
            // TODO add error handling
            break;
    }

    // Not sure how to handle GPIO more generally yet, so I'm just gonna hardcode it in here
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef gpio;
    gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1; // no idea
    gpio.Alternate = GPIO_AF4_I2C1; // no idea
    gpio.Pull = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOA, &gpio);

    hi2c.Init.Timing = 0x2000090E; // again, hardcoded...
    hi2c.Init.OwnAddress1 = own_address;
    hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    HAL_I2C_Init(&hi2c);
}

void I2C::write_reg(uint8_t addr, uint8_t reg, uint8_t val) {
    HAL_I2C_Mem_Write(&hi2c, addr, reg, 1, &val, 1, 200);
}

void I2C::read_reg(uint8_t addr, uint8_t reg, uint8_t* val) {
    HAL_I2C_Mem_Read(&hi2c, addr, reg, 1, val, 1, 200);
}

I2C_HandleTypeDef* I2C::get_i2c_handle() {
    return &hi2c;
}