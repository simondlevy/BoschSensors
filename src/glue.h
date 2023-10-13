#include "vendor/bstdr_types.h"
#include "vendor/bmi088.h"
#include "vendor/bmp3.h"

bstdr_ret_t i2c_burst_read(uint8_t dev_id, uint8_t reg_addr,
                              uint8_t *reg_data, uint16_t len);

bstdr_ret_t i2c_burst_write(uint8_t dev_id, uint8_t reg_addr,
                               uint8_t *reg_data, uint16_t len);

void delay(uint32_t msec);
