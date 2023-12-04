#include <Arduino.h>
#include <Wire.h>

#include "bmi088_i2c.hpp"

static bool okay;

static BMI088_I2C sensor;

void setup(void)
{
    Serial.begin(115200);

    Wire.begin();

    delay(100);

    okay = sensor.begin(0x19);

}

void loop(void)
{
    Serial.println(okay);

    delay(500);
}

