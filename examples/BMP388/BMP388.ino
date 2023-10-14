#include <Arduino.h>
#include <Wire.h>

#include <bmp388.hpp>

static BMP388 sensor;

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

