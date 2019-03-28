#include "stm32f3xx.h"

/*
 * I2C driver class.
 * Does not fully support all I2C features
 * Frankly would probably be better as a straight C library, but hey why not
 */

class I2C {
    public:
        void write_reg(uint8_t addr, uint8_t reg, uint8_t val);
        void read_reg(uint8_t addr, uint8_t reg, uint8_t* val);

        I2C();
        I2C(uint8_t instance_num, uint32_t frequency, uint8_t own_address);

    private:
        I2C_HandleTypeDef hi2c;
};