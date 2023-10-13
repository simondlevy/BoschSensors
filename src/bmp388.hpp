extern "C" {

#include "vendor/bstdr_types.h"
#include "vendor/bmp3.h"
}

#include "glue.h"

class BMP388 {

    public:

        bool init(void)
        {
            _dev.dev_id = BMP3_I2C_ADDR_SEC;
            _dev.intf = BMP3_I2C_INTF;
            _dev.read = (bmp3_com_fptr_t)i2c_burst_read;
            _dev.write = (bmp3_com_fptr_t)i2c_burst_write;
            _dev.delay_ms = delay;

            return bmp3_init(&_dev) == BMP3_OK;
        }

        void read(float & pressure, float & temperature)
        {

            bmp3_get_sensor_data(BMP3_PRESS | BMP3_TEMP, &_data, &_dev);

            pressure = _data.pressure;
            temperature = _data.temperature;
        }

        bool set(void)
        {
            // Select the pressure and temperature sensor to be enabled
            _dev.settings.press_en = BMP3_ENABLE;
            _dev.settings.temp_en = BMP3_ENABLE;

            // Select the output data rate and oversampling settings for pressure
            // and temperature
            _dev.settings.odr_filter.press_os = BMP3_OVERSAMPLING_8X;
            _dev.settings.odr_filter.temp_os = BMP3_NO_OVERSAMPLING;
            _dev.settings.odr_filter.odr = BMP3_ODR_50_HZ;
            _dev.settings.odr_filter.iir_filter = BMP3_IIR_FILTER_COEFF_3;

            // Assign the settings which needs to be set in the sensor */
            const uint16_t settings_sel = BMP3_PRESS_EN_SEL | BMP3_TEMP_EN_SEL
                | BMP3_PRESS_OS_SEL | BMP3_TEMP_OS_SEL | BMP3_ODR_SEL |
                BMP3_IIR_FILTER_SEL;

            _dev.settings.op_mode = BMP3_NORMAL_MODE;

            return bmp3_set_sensor_settings(settings_sel, &_dev) == BMP3_OK ?
                bmp3_set_op_mode(&_dev) == BMP3_OK :
                false;
        }


    private:

        typedef struct bmp3_dev dev_t;

        typedef  struct bmp3_data data_t;

        dev_t _dev;

        data_t _data;
};
