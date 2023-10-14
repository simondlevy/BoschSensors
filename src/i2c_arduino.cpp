#include <Wire.h>

#include "vendor/bstdr_types.h"

bstdr_ret_t i2c_burst_read(uint8_t dev_id, uint8_t reg_addr, 
        uint8_t *reg_data, uint16_t len) {

    int status = 0;

    //Loop until the port is transmitted correctly
    do
    {
        Wire.beginTransmission(dev_id);
        Wire.write(&reg_addr, 1);
        status = Wire.endTransmission(false);
        //Fix for some STM32 boards
        //Reinitialize th i2c bus with the default parameters
#ifdef ARDUINO_ARCH_STM32
        if (status)
        {
            Wire.end();
            Wire.begin();
        }
#endif
        //End of fix
    }
    while(status != 0);

    Wire.requestFrom((int)dev_id, (int) len);

    int i=0;
    while (Wire.available()) {
        reg_data[i] = Wire.read();
        i++;
    }

    return BSTDR_OK;
}

bstdr_ret_t i2c_burst_write(uint8_t dev_id, uint8_t reg_addr, 
        uint8_t *reg_data, uint16_t len) {

    Wire.beginTransmission(dev_id);
    Wire.write(&reg_addr, 1);
    for (uint16_t i = 0 ; i < len ; i++)
        Wire.write(reg_data[i]);

    Wire.endTransmission(true);

    return BSTDR_OK;
}
