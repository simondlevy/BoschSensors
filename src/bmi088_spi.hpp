#pragma once

#include "bmi088.hpp"

class BMI088_SPI : public BMI088 {

    public:

        void init(void)
        {
            BMI088::init();

            _dev.gyro_id = BMI088_GYRO_I2C_ADDR_PRIMARY;
            _dev.interface = BMI088_SPI_INTF;

            _dev.read = (bmp3_com_fptr_t)spi_burst_read;
            _dev.write = (bmp3_com_fptr_t)spi_burst_write;
        }
};
