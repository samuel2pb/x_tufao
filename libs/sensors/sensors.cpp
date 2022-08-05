extern "C"
{
#include "sensors.h"
#include <Arduino.h>
}

int pin = 12;

float _dht22_::read_env_temp()
{

    float temperature, humidity;

    dht22(pin).read2(&temperature, &humidity, NULL);
    return temperature; /* lê e retorna temperatura e umidade ambiente */
}

float _dht22_::read_env_humidity()
{

    float temperature, humidity;

    dht22(pin).read2(&temperature, &humidity, NULL);
    return humidity; /* lê e retorna temperatura e umidade ambiente */
}
