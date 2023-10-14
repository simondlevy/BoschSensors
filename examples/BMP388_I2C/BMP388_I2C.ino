#include <Arduino.h>
#include <Wire.h>

#include <bmp388_i2c.hpp>

static BMP388_I2C sensor;

void setup(void)
{
    Wire.begin();

    sensor.init();
}

void loop(void)
{
    float pressure = 0;
    float temperature = 0;

    sensor.read(pressure, temperature);

    Serial.print("Pressure = ");
    Serial.print((uint32_t)pressure);
    Serial.print(" Pa    Temperature = ");
    Serial.print(temperature);
    Serial.println(" deg C");
}

