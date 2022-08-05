#ifndef sensors_h
#define sensors_h

#include <Arduino.h>
#include <SimpleDHT.h>
#define pinDHT22 12
SimpleDHT22 dht22(pinDHT22);

class sensor_dht22
{
public:
    float read_env_temp();
    float read_env_humidity();
};

#endif