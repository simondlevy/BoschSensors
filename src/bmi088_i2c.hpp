#pragma once

#include "bmi088.hpp"

class BMI088_I2C : public BMI088 {

    public:

        void init(void)
        {
            BMI088::init();

            _dev.gyro_id = BMI088_GYRO_I2C_ADDR_SECONDARY;
            _dev.interface = BMI088_I2C_INTF;

            _dev.read = (bmp3_com_fptr_t)i2c_burst_read;
            _dev.write = (bmp3_com_fptr_t)i2c_burst_write;
        }
};
