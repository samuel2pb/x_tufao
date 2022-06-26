/*inclusão das bibliotecas necessárias*/
#include <OneWire.h>  
#include <DallasTemperature.h>

#define dados 7   /*o pino de dados do sensor está ligado na porta 7 do Arduino */

OneWire oneWire(dados);   /* Protocolo OneWire */

DallasTemperature sensors(&oneWire);    /*encaminha referências OneWire para o sensor */

void setup(void)
{ 
 Serial.begin(9600);
 sensors.begin();
} 
void loop(void)
{ 
 sensors.requestTemperatures(); /* Envia o comando para leitura da temperatura */
 Serial.println(sensors.getTempCByIndex(0)); /* Endereço do sensor */
 delay(1000); /* Delay de 1 segundo para as leituras*/
}
