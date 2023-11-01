#include "bosch/bstdr_types.h"
#include "bosch/bmi088_defs.h"

bstdr_ret_t i2c_burst_read(uint8_t dev_id, uint8_t reg_addr,
                              uint8_t *reg_data, uint16_t len);

bstdr_ret_t i2c_burst_write(uint8_t dev_id, uint8_t reg_addr,
                               uint8_t *reg_data, uint16_t len);

void sensorsBmi088_I2C_deviceInit(bmi088_dev_t *device)
{
  device->gyro_id = BMI088_GYRO_I2C_ADDR_SECONDARY;
  device->interface = BMI088_I2C_INTF;
  device->read = (bmi088_com_fptr_t)i2c_burst_read;
  device->write = (bmi088_com_fptr_t)i2c_burst_write;
}
