extern "C" {

#include "vendor/bstdr_types.h"
#include "vendor/bmi088.h"
#include "vendor/bmp3.h"
}

#include "glue.h"

class BMI088 {

    public:

        static const uint8_t SENSORS_BMI088_GYRO_FS_CFG = BMI088_GYRO_RANGE_2000_DPS;
        static const uint8_t SENSORS_BMI088_ACCEL_FS_CFG = BMI088_ACCEL_RANGE_24G;

        void init(void)
        {
            _dev.accel_id = BMI088_ACCEL_I2C_ADDR_PRIMARY;
            _dev.delay_ms = delay;
        }

        void i2cInit(void)
        {
            init();

            _dev.gyro_id = BMI088_GYRO_I2C_ADDR_SECONDARY;
            _dev.interface = BMI088_I2C_INTF;

            _dev.read = (bmp3_com_fptr_t)i2c_burst_read;
            _dev.write = (bmp3_com_fptr_t)i2c_burst_write;
        }

        void spiInit(void)
        {
            init();

            _dev.gyro_id = BMI088_GYRO_I2C_ADDR_PRIMARY;
            _dev.interface = BMI088_SPI_INTF;

            _dev.read = (bmp3_com_fptr_t)spi_burst_read;
            _dev.write = (bmp3_com_fptr_t)spi_burst_write;
        }

        bool readGyro(struct bmi088_sensor_data * dataOut)
        {
            return bmi088_get_gyro_data(dataOut, &_dev) == BMI088_OK;
        }

        bool readAccel(struct bmi088_sensor_data * dataOut)
        {
            return bmi088_get_accel_data(dataOut, &_dev) == BMI088_OK;
        }

        bool initGyro(void)
        {
            auto rslt = bmi088_gyro_init(&_dev);

            struct bmi088_int_cfg intConfig;

            // Set power mode
            _dev.gyro_cfg.power = BMI088_GYRO_PM_NORMAL;
            rslt |= bmi088_set_gyro_power_mode(&_dev);

            // Set bandwidth and range
            _dev.gyro_cfg.bw = BMI088_GYRO_BW_116_ODR_1000_HZ;
            _dev.gyro_cfg.range = SENSORS_BMI088_GYRO_FS_CFG;
            _dev.gyro_cfg.odr = BMI088_GYRO_BW_116_ODR_1000_HZ;
            rslt |= bmi088_set_gyro_meas_conf(&_dev);

            intConfig.gyro_int_channel = BMI088_INT_CHANNEL_3;
            intConfig.gyro_int_type = BMI088_GYRO_DATA_RDY_INT;
            intConfig.gyro_int_pin_3_cfg.enable_int_pin = 1;
            intConfig.gyro_int_pin_3_cfg.lvl = 1;
            intConfig.gyro_int_pin_3_cfg.output_mode = 0;

            // Set interrupt configuration
            rslt |= bmi088_set_gyro_int_config(&intConfig, &_dev);

            delay(50);

            struct bmi088_sensor_data gyr = {};

            rslt |= bmi088_get_gyro_data(&gyr, &_dev);

            return rslt == BMI088_OK;
        }

        bool initAccel(void)
        {
            auto rslt = bmi088_accel_switch_control(&_dev, BMI088_ACCEL_POWER_ENABLE);

            rslt |= bmi088_accel_init(&_dev);

            // Set power mode
            _dev.accel_cfg.power = BMI088_ACCEL_PM_ACTIVE;
            rslt |= bmi088_set_accel_power_mode(&_dev);
            delay(10);

            // Set bandwidth and range
            _dev.accel_cfg.bw = BMI088_ACCEL_BW_OSR4;
            _dev.accel_cfg.range = SENSORS_BMI088_ACCEL_FS_CFG;
            _dev.accel_cfg.odr = BMI088_ACCEL_ODR_1600_HZ;
            rslt |= bmi088_set_accel_meas_conf(&_dev);

            struct bmi088_sensor_data acc;
            rslt |= bmi088_get_accel_data(&acc, &_dev);

            return rslt == BMI088_OK;
        }

        bool runGyroSelfTest(void)
        {
            int8_t tmp = 0;

            return bmi088_perform_gyro_selftest(&tmp, &_dev) ==
                BMI088_SELFTEST_PASS;
        }

        bool runAccelSelfTest(void)
        {
            int8_t tmp = 0;

            return bmi088_perform_accel_selftest(&tmp, &_dev) ==
                BMI088_SELFTEST_PASS;
        }

    private:

        struct bmi088_dev _dev;

};
