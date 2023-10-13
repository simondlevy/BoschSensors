extern "C" {

#include "vendor/bstdr_types.h"
#include "vendor/bmi088.h"
#include "vendor/bmp3.h"
}

#include "glue.h"

class BMI088 {

    public:

        static const uint8_t SENSORS_BMI088_GYRO_FS_CFG = BMI088_GYRO_RANGE_2000_DPS;

        static void i2cInit(struct bmi088_dev *device)
        {
            device->accel_id = BMI088_ACCEL_I2C_ADDR_PRIMARY;
            device->gyro_id = BMI088_GYRO_I2C_ADDR_SECONDARY;
            device->interface = BMI088_I2C_INTF;
            device->read = (bmp3_com_fptr_t)i2c_burst_read;
            device->write = (bmp3_com_fptr_t)i2c_burst_write;
            device->delay_ms = delay;
        }

        static bool readGyro(
                struct bmi088_sensor_data * dataOut, 
                bmi088_dev * bmi088Dev)
        {
            return bmi088_get_gyro_data(dataOut, bmi088Dev) == BMI088_OK;
        }

        static bool readAccel(
                struct bmi088_sensor_data * dataOut, 
                bmi088_dev * bmi088Dev)
        {
            return bmi088_get_accel_data(dataOut, bmi088Dev) == BMI088_OK;
        }

        static bool initGyro(bmi088_dev * bmi088Dev)
        {
            auto rslt = bmi088_gyro_init(bmi088Dev);

            struct bmi088_int_cfg intConfig;

            // Set power mode
            bmi088Dev->gyro_cfg.power = BMI088_GYRO_PM_NORMAL;
            rslt |= bmi088_set_gyro_power_mode(bmi088Dev);

            // Set bandwidth and range
            bmi088Dev->gyro_cfg.bw = BMI088_GYRO_BW_116_ODR_1000_HZ;
            bmi088Dev->gyro_cfg.range = SENSORS_BMI088_GYRO_FS_CFG;
            bmi088Dev->gyro_cfg.odr = BMI088_GYRO_BW_116_ODR_1000_HZ;
            rslt |= bmi088_set_gyro_meas_conf(bmi088Dev);

            intConfig.gyro_int_channel = BMI088_INT_CHANNEL_3;
            intConfig.gyro_int_type = BMI088_GYRO_DATA_RDY_INT;
            intConfig.gyro_int_pin_3_cfg.enable_int_pin = 1;
            intConfig.gyro_int_pin_3_cfg.lvl = 1;
            intConfig.gyro_int_pin_3_cfg.output_mode = 0;

            // Set interrupt configuration
            rslt |= bmi088_set_gyro_int_config(&intConfig, bmi088Dev);

            delay(50);

            struct bmi088_sensor_data gyr = {};

            rslt |= bmi088_get_gyro_data(&gyr, bmi088Dev);

            return rslt == BMI088_OK;
        }
};

