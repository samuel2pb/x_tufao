#ifndef SENSORS_H_INCLUDED
#define SENSORS_H_INCLUDED

class dht22
{
#include <SimpleDHT.h>
#include <Arduino.h>

public:
    dht22(); // Constructor
    int pin;
    SimpleDHT22 dht(int);
    float read_env_temp();
    float read_env_humidity();

private:
    // Variables for DHT22
};

#endif