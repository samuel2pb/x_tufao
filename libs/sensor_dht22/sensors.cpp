#include "sensors.h"

float read_env_temp()
{

    float temperature, humidity;

    dht22.read2(&temperature, &humidity, NULL);
    return temperature; /* lê e retorna temperatura e umidade ambiente */
}

float read_env_humidity()
{

    float temperature, humidity;

    dht22.read2(&temperature, &humidity, NULL);
    return humidity; /* lê e retorna temperatura e umidade ambiente */
}
