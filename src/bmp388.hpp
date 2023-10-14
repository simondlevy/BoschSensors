extern "C" {

#include "vendor/bstdr_types.h"
#include "vendor/bmp3.h"
}

#include "glue.h"

class BMP388 {

    public:

        typedef enum {

            PRESSURE_OVERSAMPLING_NONE,
            PRESSURE_OVERSAMPLING_2X,
            PRESSURE_OVERSAMPLING_4X,
            PRESSURE_OVERSAMPLING_8X,
            PRESSURE_OVERSAMPLING_16X,
            PRESSURE_OVERSAMPLING_32,

        } pressureOversampling_t;

        typedef enum {

            TEMPERATURE_OVERSAMPLING_NONE,
            TEMPERATURE_OVERSAMPLING_2X,
            TEMPERATURE_OVERSAMPLING_4X,
            TEMPERATURE_OVERSAMPLING_8X,
            TEMPERATURE_OVERSAMPLING_16X,
            TEMPERATURE_OVERSAMPLING_32,

        } temperatureOversampling_t;

        typedef enum {

            ODR_200_HZ,
            ODR_100_HZ,
            ODR_50_HZ,
            ODR_25_HZ,
            ODR_12_5_HZ,
            ODR_6_25_HZ,
            ODR_3_1_HZ,
            ODR_1_5_HZ,
            ODR_0_78_HZ,
            ODR_0_39_HZ,
            ODR_0_2_HZ,
            ODR_0_1_HZ,
            ODR_0_05_HZ,
            ODR_0_02_HZ,
            ODR_0_01_HZ,
            ODR_0_006_HZ,
            ODR_0_003_HZ,
            ODR_0_001_HZ,

        } odr_t;

        typedef enum {

            IIR_FILTER_DISABLE,
            IIR_FILTER_COEFF_1,
            IIR_FILTER_COEFF_3,
            IIR_FILTER_COEFF_7,
            IIR_FILTER_COEFF_15,
            IIR_FILTER_COEFF_31,
            IIR_FILTER_COEFF_63,
            IIR_FILTER_COEFF_127,

        } iirFilterCoeff_t;

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
            _dev.delay_ms = delay;

            if (bmp3_init(&_dev) != BMP3_OK) {
                return false;
            }

            // Select the pressure and temperature sensor to be enabled
            _dev.settings.press_en = BMP3_ENABLE;
            _dev.settings.temp_en = BMP3_ENABLE;

            // Select the output data rate and oversampling settings for pressure
            // and temperature
            _dev.settings.odr_filter.press_os = pressureOversampling; 
            _dev.settings.odr_filter.temp_os = temperatureOversampling; 
            _dev.settings.odr_filter.odr = odr; 
            _dev.settings.odr_filter.iir_filter = iirFilterCoeff; 

            // Assign the settings which needs to be set in the sensor */
            const uint16_t settings_sel = BMP3_PRESS_EN_SEL | BMP3_TEMP_EN_SEL
                | BMP3_PRESS_OS_SEL | BMP3_TEMP_OS_SEL | BMP3_ODR_SEL |
                BMP3_IIR_FILTER_SEL;

            _dev.settings.op_mode = BMP3_NORMAL_MODE;

            return bmp3_set_sensor_settings(settings_sel, &_dev) == BMP3_OK ?
                bmp3_set_op_mode(&_dev) == BMP3_OK :
                false;
        }

        void read(float & pressure, float & temperature)
        {

            bmp3_get_sensor_data(BMP3_PRESS | BMP3_TEMP, &_data, &_dev);

            pressure = _data.pressure;
            temperature = _data.temperature;
        }

    protected:

        struct bmp3_dev _dev;

    private:

        typedef  struct bmp3_data data_t;

        data_t _data;
};
