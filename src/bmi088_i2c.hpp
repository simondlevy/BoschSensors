#pragma once

#include "bosch/bstdr_types.h"

#include "bosch/bmi088_defs.h"

#include "bosch/bmi088.h"

bstdr_ret_t i2c_burst_read(uint8_t dev_id, uint8_t reg_addr,
                              uint8_t *reg_data, uint16_t len);

bstdr_ret_t i2c_burst_write(uint8_t dev_id, uint8_t reg_addr,
                               uint8_t *reg_data, uint16_t len);

class BMI088_I2C {

    public:

        bool begin(
                const uint8_t addr = BMI088_GYRO_I2C_ADDR_PRIMARY,
                const uint8_t gyroFs=BMI088_GYRO_RANGE_2000_DPS)
        {
            _dev.gyro_id = addr;
            _dev.interface = BMI088_I2C_INTF;
            _dev.read = (bmi088_com_fptr_t)i2c_burst_read;
            _dev.write = (bmi088_com_fptr_t)i2c_burst_write;

            _dev.delay_ms = delay;

            auto rslt = bmi088_gyro_init(&_dev);

            if (rslt == BSTDR_OK) {

                struct bmi088_int_cfg intConfig;

                _dev.gyro_cfg.power = BMI088_GYRO_PM_NORMAL;
                rslt |= bmi088_set_gyro_power_mode(&_dev);

                _dev.gyro_cfg.bw = BMI088_GYRO_BW_116_ODR_1000_HZ;
                _dev.gyro_cfg.range = gyroFs;
                _dev.gyro_cfg.odr = BMI088_GYRO_BW_116_ODR_1000_HZ;
                rslt |= bmi088_set_gyro_meas_conf(&_dev);

                intConfig.gyro_int_channel = BMI088_INT_CHANNEL_3;
                intConfig.gyro_int_type = BMI088_GYRO_DATA_RDY_INT;
                intConfig.gyro_int_pin_3_cfg.enable_int_pin = 1;
                intConfig.gyro_int_pin_3_cfg.lvl = 1;
                intConfig.gyro_int_pin_3_cfg.output_mode = 0;

                rslt = bmi088_set_gyro_int_config(&intConfig, &_dev);

                delay(50);
                struct bmi088_sensor_data gyr;
                rslt |= bmi088_get_gyro_data(&gyr, &_dev);
            }

            return rslt == BSTDR_OK;
        }

    private:

        bmi088_dev_t _dev;

};
