#pragma once

#include "glue.h"

class BMP388_I2C : public BMP388 {

    public:

        bool init(
                pressureOversampling_t pressureOversampling
                = PRESSURE_OVERSAMPLING_8X
                , temperatureOversampling_t temperatureOversampling
                = TEMPERATURE_OVERSAMPLING_NONE
                , odr_t odr = ODR_50_HZ
                , iirFilterCoeff_t iirFilterCoeff = IIR_FILTER_COEFF_3
                )
        {
            _dev.dev_id = BMP3_I2C_ADDR_SEC;
            _dev.intf = BMP3_I2C_INTF;
            _dev.read = (bmp3_com_fptr_t)i2c_burst_read;
            _dev.write = (bmp3_com_fptr_t)i2c_burst_write;

            return BMP388::init(
                    pressureOversampling,
                    temperatureOversampling,
                    odr,
                    iirFilterCoeff);
        }
};
