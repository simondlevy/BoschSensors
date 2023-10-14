extern "C" {

#include "vendor/bstdr_types.h"
#include "vendor/bmi088.h"
#include "vendor/bmp3.h"
}

#include "glue.h"

class BMI088 {

    public:

        typedef enum {

            GYRO_RANGE_2000_DPS,
            GYRO_RANGE_1000_DPS,
            GYRO_RANGE_500_DPS,
            GYRO_RANGE_250_DPS,
            GYRO_RANGE_125_DPS,

        } gyroRange_t;

        typedef enum {

            GYRO_BW_532_ODR_2000_HZ,
            GYRO_BW_230_ODR_2000_HZ,
            GYRO_BW_116_ODR_1000_HZ,
            GYRO_BW_47_ODR_400_HZ,
            GYRO_BW_23_ODR_200_HZ,
            GYRO_BW_12_ODR_100_HZ,
            GYRO_BW_64_ODR_200_HZ,
            GYRO_BW_32_ODR_100_HZ,

        } gyroBwOdr_t;

        typedef enum {

            ACCEL_RANGE_3G,
            ACCEL_RANGE_6G,
            ACCEL_RANGE_12G,
            ACCEL_RANGE_24G,

        } accelRange_t;

        typedef enum {

            ACCEL_BW_OSR4,    
            ACCEL_BW_OSR2,   
            ACCEL_BW_NORMAL,

        } accelBwOsr_t;

        typedef enum {

            ACCEL_ODR_12_5_HZ = 5,
            ACCEL_ODR_25_HZ,      
            ACCEL_ODR_50_HZ,     
            ACCEL_ODR_100_HZ,   
            ACCEL_ODR_200_HZ,  
            ACCEL_ODR_400_HZ, 
            ACCEL_ODR_800_HZ,
            ACCEL_ODR_1600_HZ, 

        } accelOdr_t;

        typedef enum {

            GYRO_POWER_MODE_NORMAL       = 0x00,
            GYRO_POWER_MODE_DEEP_SUSPEND = 0x20,
            GYRO_POWER_MODE_SUSPEND      = 0x80,

        } gyroPowerMode_t;

        typedef enum {

            ACCEL_POWER_MODE_ACTIVE  = 0x00,
            ACCEL_POWER_MODE_SUSPEND = 0x03,

        } accelPowerMode_t;

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


        bool initGyro(
                const gyroBwOdr_t bwOdr=GYRO_BW_116_ODR_1000_HZ
                , const gyroRange_t range=GYRO_RANGE_2000_DPS
                , const gyroPowerMode_t powerMode=GYRO_POWER_MODE_NORMAL
                )
        {
            auto rslt = bmi088_gyro_init(&_dev);

            struct bmi088_int_cfg intConfig;

            // Set power mode
            _dev.gyro_cfg.power = powerMode;
            rslt |= bmi088_set_gyro_power_mode(&_dev);

            // Set bandwidth and range
            _dev.gyro_cfg.bw = bwOdr;
            _dev.gyro_cfg.range = range;
            _dev.gyro_cfg.odr = bwOdr;
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

        bool initAccel(
                const accelRange_t range=ACCEL_RANGE_24G
                , const accelBwOsr_t bwOsr=ACCEL_BW_OSR4
                , const accelOdr_t odr=ACCEL_ODR_1600_HZ
                , const accelPowerMode_t powerMode=ACCEL_POWER_MODE_ACTIVE
                )
        {
            auto rslt = bmi088_accel_switch_control(&_dev, BMI088_ACCEL_POWER_ENABLE);

            rslt |= bmi088_accel_init(&_dev);

            // Set power mode
            _dev.accel_cfg.power = powerMode;
            rslt |= bmi088_set_accel_power_mode(&_dev);
            delay(10);

            // Set bandwidth and range
            _dev.accel_cfg.bw = bwOsr;
            _dev.accel_cfg.range = range;
            _dev.accel_cfg.odr = odr;
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
