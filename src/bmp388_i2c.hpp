#pragma once

#include "bmp388.hpp"
#include "glue.h"

class BMP388_I2C : public BMP388 {

    protected:

        virtual void devInit(BMP388::dev_t & dev) override
        {
            dev.dev_id = BMP3_I2C_ADDR_PRIM;
            dev.intf = BMP3_I2C_INTF;
            dev.read = (bmp3_com_fptr_t)i2c_burst_read;
            dev.write = (bmp3_com_fptr_t)i2c_burst_write;
        }
};
