#ifndef SENSORS_H_INCLUDED
#define SENSORS_H_INCLUDED

#include <SimpleDHT.h>

class dht22
{
private:
    // Variables for DHT22
public:
    dht22(); // Constructor
    int pin;
    SimpleDHT22 dht(int pin);
    float temperature, humidity;
    float read_env_temp();
    float read_env_humidity();
};

#endif