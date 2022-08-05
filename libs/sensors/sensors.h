#ifndef sensors_h
#define sensors_h

extern "C"
{
#include <SimpleDHT.h>
#include <Arduino.h>
}

//#define pinDHT22 12
// SimpleDHT22 dht22(pinDHT22);

class _dht22_
{
private:
    // Variables for DHT22

    // Constructor
public:
    SimpleDHT22 dht22(int pinDHT22);
    float read_env_temp();
    float read_env_humidity();
};

#endif