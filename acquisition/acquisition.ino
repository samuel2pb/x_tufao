
/*------------------------------------- Libraries ----------------------------------------------------*/

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <GravityTDS.h>
#include <EEPROM.h>
#include <DHT.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
/*------------------------------------- Definitions ----------------------------------------------------*/

#define dados 12    /*o pino de dados do sensor está ligado na porta 7 do Arduino */  
#define TdsSensorPin A0 /* o pino de dados do sensor de turbidez */
#define pinNivel 27 /* o pino de dados do sensor de nível tipo boia */
#define pinDHT22 13 /* o pino de dados do sensor de temperatura ambiente DHT22 */
#define DHTTYPE DHT22   // Sensor DHT 22  (AM2302)


/*------------------------------------- Library Objects --------------------------------------------*/

OneWire oneWire(dados);              /* Protocolo OneWire */
DallasTemperature wts(&oneWire); /*encaminha referências OneWire para o sensor */
GravityTDS tds;                   /* objeto eletrocondutividade da biblioteca Gravity*/
DHT dht(pinDHT22, DHTTYPE);        /* objeto temperatura e umidade da biblioteca SimpleDHT */

/*------------------------------------- Global Variables ------------------------------------------*/ 

float env_hum = 0;
float env_temp = 0;
float water_temp = 0;
float ec_value = 0;
int state = 0;

/*------------------------------------- Sensors ----------------------------------------------------*/

void sensors( float &env_temp , float &env_hum, float &water_temp, float &ec_value, int &state)
{
  env_hum = dht.readHumidity();
  env_temp = dht.readTemperature();
  wts.requestTemperatures(); /* Envia o comando para leitura da temperatura */
  water_temp = wts.getTempCByIndex(0); /* Endereço do sensor */
  tds.setTemperature(water_temp); /* Seta temperatura e efetua compensacao */
  tds.update();
  ec_value = tds.getEcValue();
  state = digitalRead(pinNivel);
}

/*--------------------------------------Actuators--------------------------------------*/




/*--------------------------------------Setup------------------------------------------*/

void setup(void)
{
  Serial.begin(115200);
  pinMode(pinNivel, INPUT); /*Pino do sensor de nível como entrada de sinal*/
  wts.begin();
  tds.setPin(TdsSensorPin);
  tds.setAref(5.0);      /* Tensao de referencia ADC, Padrao de 5.0 V para arduino ou 3.3 para ESP32 */
  tds.setAdcRange(1024); /* Range de 1024 para 10bit ADC; 4096 para 12bit ADC */
  tds.begin();           /* Inicializa o TDS*/
  dht.begin();           /* Inicializa o TDS*/
}

/*-----------------------------------Loop---------------------------------------------*/

void loop(void)
{

  sensors(env_temp, env_hum, water_temp, ec_value, state);
  /*
  if (isnan(env_temp) || isnan(env_hum) || isnan(water_temp) || isnan(ec_value) || isnan(state) )
  {
    Serial.println(F("Failed to read from sensors!"));
    return;
  }*/  /*Checa se alguma leitura falha, retorna para uma nova leitura */
  
  Serial.print(F("%  Env_Temperature: "));
  Serial.print(env_temp);
  Serial.println(F("°C "));
  Serial.print(F("Humidity: "));
  Serial.print(env_hum);
  Serial.print(F("%  Water_Temperature: "));
  Serial.print(water_temp);
  Serial.println(F("°C "));
  Serial.print(F("%  EC_Value: "));
  Serial.print(ec_value);
  Serial.println(F("uS/cm "));
  Serial.print(F("%  Water_Level: "));
  Serial.print(state);
 
  delay(5000);
}
