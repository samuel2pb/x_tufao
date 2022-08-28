
/*inclusão das bibliotecas necessárias*/
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Arduino.h>
#include <DFRobot_ESP_EC.h>
#include <EEPROM.h>
#include "SimpleDHT.h"

extern "C"
{
#include "libs/sensors/sensors.h"
}

#define dados 14    /*o pino de dados do sensor está ligado na porta 7 do Arduino */
#define pinTDS 13   /* o pino de dados do sensor de turbidez */
#define pinDHT22 12 /* o pino de dados do sensor de temperatura ambiente DHT22 */
#define pinNivel 27 /* o pino de dados do sensor de nível tipo boia */

OneWire oneWire(dados);              /* Protocolo OneWire */
DallasTemperature sensors(&oneWire); /*encaminha referências OneWire para o sensor */
DFRobot_ESP_EC ec;                   /* objeto eletrocondutividade da biblioteca DFrobot*/
SimpleDHT22 dht22(pinDHT22);         /* objeto temperatura e umidade da biblioteca SimpleDHT */

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

float read_water_Temperature()
{
  sensors.requestTemperatures();
  /* Envia o comando para leitura da temperatura */
  return sensors.getTempCByIndex(0); /* Endereço do sensor */
}

float read_EC()
{
  float voltage, ecValue, temperature = 25;

  voltage = analogRead(pinTDS);
  // Serial.print("voltage:");
  // Serial.println(voltage, 4);

  // temperature = read_water_Temperature();  /* le temperatura para executar compensacao */
  // Serial.print("temperature:");
  // Serial.print(temperature, 1);
  // Serial.println("C");

  ecValue = ec.readEC(voltage, temperature); /* converte tensao para condutividade utilizando compensacao de temperatura */
  // Serial.print("EC:");
  // Serial.print(ecValue, 4);
  // Serial.println("ms/cm");
  // Serial.println(ecValue);
  ec.calibration(voltage, temperature); /* processo de calibracao via Serail CMD */

  return ecValue;
}

int read_nivel()
{
  int estado = digitalRead(pinNivel);
  // Serial.print("Estado sensor : ");
  // Serial.println(estado);
}

void setup(void)
{
  Serial.begin(115200);
  pinMode(pinNivel, INPUT); /*Pino do sensor de nível como entrada de sinal*/
  EEPROM.begin(32);
  ec.begin();
  sensors.begin();
}
void loop(void)
{

  float temp_agua, temp_amb, umidade_amb, condutividade;

  temp_agua = read_water_Temperature();
  temp_amb = read_env_temp();
  umidade_amb = read_env_humidity();
  condutividade = read_EC();
  Serial.print("Temperatura Ambiente ");
  Serial.print(temp_amb);
  Serial.print("\nUmidade ambiente ");
  Serial.print(umidade_amb);
  Serial.print("\nTemperatua Agua ");
  Serial.print(temp_agua);
  Serial.print("\nCondutividade ");
  Serial.print(condutividade);
  Serial.print("\n\n");
  delay(2000);
}
