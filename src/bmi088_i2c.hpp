#pragma once

#include "bosch/bstdr_types.h"
#include "bosch/bmi088_defs.h"

bstdr_ret_t i2c_burst_read(uint8_t dev_id, uint8_t reg_addr,
                              uint8_t *reg_data, uint16_t len);

bstdr_ret_t i2c_burst_write(uint8_t dev_id, uint8_t reg_addr,
                               uint8_t *reg_data, uint16_t len);

class BMI088_I2C {

    public:

        BMI088_I2C(void)
        {
            _dev.gyro_id = BMI088_GYRO_I2C_ADDR_SECONDARY;
            _dev.interface = BMI088_I2C_INTF;
            _dev.read = (bmi088_com_fptr_t)i2c_burst_read;
            _dev.write = (bmi088_com_fptr_t)i2c_burst_write;
        }

    private:

        bmi088_dev_t _dev;

};
