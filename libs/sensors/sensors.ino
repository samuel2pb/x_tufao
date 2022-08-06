#include "sensors.h"

dht22 get_status;

void setup(void)
{
    Serial.begin(115200);
}
void loop(void)
{

    float temp_agua, temp_amb, umidade_amb, condutividade;

    temp_amb = get_status.read_env_temp();
    umidade_amb = get_status.read_env_humidity();
    Serial.print("Temperatura Ambiente ");
    Serial.print(temp_amb);
    Serial.print("\nUmidade ambiente ");
    Serial.print(umidade_amb);
    delay(2000);
}