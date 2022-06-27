
/*inclusão das bibliotecas necessárias*/
#include <OneWire.h>  
#include <DallasTemperature.h>
#include <Arduino.h> 
#include <DFRobot_ESP_EC.h>
#include <EEPROM.h>
#include <SimpleDHT.h>

#define dados 7   /*o pino de dados do sensor está ligado na porta 7 do Arduino */
#define pinTDS 13 /* o pino de dados do sensor de turbidez */
#define pinDHT22 12 /* o pino de dados do sensor de temperatura ambiente DHT22 */
#define pinNivel 8 /* o pino de dados do sensor de nível tipo boia */

OneWire oneWire(dados);   /* Protocolo OneWire */
DallasTemperature sensors(&oneWire);    /*encaminha referências OneWire para o sensor */
DFRobot_ESP_EC ec; /* objeto eletrocondutividade da biblioteca DFrobot*/
SimpleDHT22 dht22(pinDHT22);

float read_env_temp_humidity()
{
  
  float temperature = 0, humidity = 0;
  int err = SimpleDHTErrSuccess;
  
  if ((err = dht22.read2(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) 
  {
    Serial.print("Read DHT22 failed, err="); Serial.print(SimpleDHTErrCode(err));
    Serial.print(","); Serial.println(SimpleDHTErrDuration(err)); delay(2000);
    return temperature, humidity; /* lê e retorna temperatura e umidade ambiente */
  }
}

float read_water_Temperature()
{
  float temperature;
  
  temperature = sensors.requestTemperatures(); /* Envia o comando para leitura da temperatura */
  //Serial.println(sensors.getTempCByIndex(0)); /* Endereço do sensor */
  return temperature;
}

float read_EC()
{  
    float voltage, ecValue, temperature = 25;
    
    voltage = analogRead(PIN); 
    //Serial.print("voltage:");
    //Serial.println(voltage, 4);
 
    temperature = read_water_Temperature();  /* le temperatura para executar compensacao */
    //Serial.print("temperature:");
   // Serial.print(temperature, 1); 
    //Serial.println("C");
 
    ecValue = ec.readEC(voltage, temperature); /* converte tensao para condutividade utilizando compensacao de temperatura */ 
    //Serial.print("EC:");
    //Serial.print(ecValue, 4);
    //Serial.println("ms/cm");
    //Serial.println(ecValue); 
    ec.calibration(voltage, temperature); /* processo de calibracao via Serail CMD */
       
   return ecValue;
}

int read_nivel(){
  int estado = digitalRead(pinSensor);
  // Serial.print("Estado sensor : ");
  // Serial.println(estado);
}

void setup(void)
{ 
  Serial.begin(115200);
  pinMode(pinSensor, INPUT); /*Pino do sensor de nível como entrada de sinal*/
  EEPROM.begin(32);
  ec.begin(); 
  sensors.begin();
}
void loop(void)
{ 

}
