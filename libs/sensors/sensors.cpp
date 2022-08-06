#include "sensors.h"

float dht22::read_env_temp()
{
    this->pin = 12;
    float temperature, humidity;

    dht(pin).read2(&temperature, &humidity, NULL);
    return temperature; /* lê e retorna temperatura e umidade ambiente */
}

float dht22::read_env_humidity()
{
    this->pin = 12;
    float temperature, humidity;

    dht(pin).read2(&temperature, &humidity, NULL);
    return humidity; /* lê e retorna temperatura e umidade ambiente */
}
