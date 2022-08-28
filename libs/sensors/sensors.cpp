#include "sensors.h"
// Configurações do Sensor de Temperatura e Umidade DHT22

SimpleDHT22 dht22::dht(int pin)
{
    this->pin = 12;
}

float dht22::read_env_temp()
{
    this->temperature;
    this->humidity;
    dht(pin).read2(&temperature, &humidity, NULL);
    return temperature; /* lê e retorna temperatura e umidade ambiente */
}

float dht22::read_env_humidity()
{
    this->temperature;
    this->humidity;
    dht(pin).read2(&temperature, &humidity, NULL);
    return humidity; /* lê e retorna temperatura e umidade ambiente */
}