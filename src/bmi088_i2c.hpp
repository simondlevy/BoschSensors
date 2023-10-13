extern "C" {

#include "vendor/bstdr_types.h"
#include "vendor/bmi088.h"
#include "vendor/bmp3.h"
}

#include "glue.h"

void Bmi088_I2C_deviceInit(struct bmi088_dev *device)
{
  device->accel_id = BMI088_ACCEL_I2C_ADDR_PRIMARY;
  device->gyro_id = BMI088_GYRO_I2C_ADDR_SECONDARY;
  device->interface = BMI088_I2C_INTF;
  device->read = (bmp3_com_fptr_t)i2c_burst_read;
  device->write = (bmp3_com_fptr_t)i2c_burst_write;
  device->delay_ms = delay;
}
