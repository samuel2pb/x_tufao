
/*------------------------------------- Libraries ----------------------------------------------------*/

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <GravityTDS.h>
#include <EEPROM.h>
#include <SimpleDHT.h>
#include "libs/secrets.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>

/*------------------------------------- Definitions ----------------------------------------------------*/

#define dados 14    /*o pino de dados do sensor está ligado na porta 7 do Arduino */  
#define TdsSensorPin 15 /* o pino de dados do sensor de turbidez */
#define pinNivel 27 /* o pino de dados do sensor de nível tipo boia */
#define pinDHT22 13 /* o pino de dados do sensor de temperatura ambiente DHT22 */
#define AWS_IOT_PUBLISH_TOPIC   "esp_dht/pub" /* Tópico de publicacao na AWS */ 
#define AWS_IOT_SUBSCRIBE_TOPIC "esp_dht/sub" /* Tópico de assinatura na AWS */


/*------------------------------------- Library Objects --------------------------------------------*/

OneWire oneWire(dados);              /* Protocolo OneWire */
DallasTemperature wts(&oneWire); /*encaminha referências OneWire para o sensor */
GravityTDS tds;                   /* objeto eletrocondutividade da biblioteca Gravity*/
SimpleDHT22 dht22(pinDHT22);         /* objeto temperatura e umidade da biblioteca SimpleDHT */
WiFiClientSecure net = WiFiClientSecure(); /* objeto client secure */
PubSubClient client(net); /* objeto pubsub */

/*------------------------------------- Global Variables ------------------------------------------*/ 

float env_hum = 0;
float env_temp = 0;
float water_temp = 0;
float ec_value = 0;
int state = 0;

/*------------------------------------- Sensors ----------------------------------------------------*/

void sensors( float &env_temp , float &env_hum, float &water_temp, float &ec_value, int &state)
{

  dht22.read2(&env_temp, &env_hum, NULL); /* lê e retorna temperatura e umidade ambiente */
  wts.requestTemperatures(); /* Envia o comando para leitura da temperatura */
  water_temp = wts.getTempCByIndex(0); /* Endereço do sensor */
  tds.setTemperature(water_temp); // set the temperature and execute temperature compensation
  tds.update();
  ec_value = tds.getEcValue();
  state = digitalRead(pinNivel);
}

/*--------------------------------------Actuators--------------------------------------*/


/*--------------------------------------IOT-Connection---------------------------------*/
void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
  Serial.println("Connecting to Wi-Fi");
 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
 
  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);
 
  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);
 
  // Create a message handler
  client.setCallback(messageHandler);
 
  Serial.println("Connecting to AWS IOT");
 
  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(100);
  }
 
  if (!client.connected())
  {
    Serial.println("AWS IoT Timeout!");
    return;
  }
 
  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
 
  Serial.println("AWS IoT Connected!");
}

void publishMessage()
{
  StaticJsonDocument<200> doc;
  doc["humidity"] = env_hum;
  doc["env_temperature"] = env_temp;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
 
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}
 
void messageHandler(char* topic, byte* payload, unsigned int length)
{
  Serial.print("incoming: ");
  Serial.println(topic);
 
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char* message = doc["message"];
  Serial.println(message);
}


/*--------------------------------------Setup------------------------------------------*/

void setup(void)
{
  Serial.begin(115200);
  pinMode(pinNivel, INPUT); /*Pino do sensor de nível como entrada de sinal*/
  EEPROM.begin(32);
  wts.begin();
  tds.setPin(TdsSensorPin);
  tds.setAref(5.0);      // reference voltage on ADC, default 5.0V on Arduino UNO
  tds.setAdcRange(1024); // 1024 for 10bit ADC;4096 for 12bit ADC
  tds.begin();           // initialization
  connectAWS(); //Connect to Amazon Web Services
}

/*-----------------------------------Loop---------------------------------------------*/

void loop(void)
{

  sensors(env_temp, env_hum, water_temp, ec_value, state);
  
  if (isnan(env_hum) || isnan(env_temp) )  // Check if any reads failed and exit early (to try again).
  {
    Serial.println(F("Failed to read from sensors!"));
    return;
  }
 
  Serial.print(F("Humidity: "));
  Serial.print(env_hum);
  Serial.print(F("%  Temperature: "));
  Serial.print(env_temp);
  Serial.println(F("°C "));
 
  publishMessage();
  client.loop();
  delay(5000);
}
