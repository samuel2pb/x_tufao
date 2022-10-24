
/*------------------------------------- Libraries ----------------------------------------------------*/

#include <Arduino.h>
#include "libs/secrets.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HardwareSerial.h>

/*------------------------------------- Definitions ----------------------------------------------------*/

#define AWS_IOT_PUBLISH_TOPIC   "esp_dht/pub" /* Tópico de publicacao na AWS */ 
#define AWS_IOT_SUBSCRIBE_TOPIC "esp_dht/sub" /* Tópico de assinatura na AWS */
#define rxPin 16
#define txPin 17


/*------------------------------------- Library Objects --------------------------------------------*/

WiFiClientSecure net = WiFiClientSecure(); /* objeto client secure */
PubSubClient client(net); /* objeto pubsub */
HardwareSerial SerialPort(2); // Esta sendo utilizado a UART 2 (pinos 16 e 17 do esp rx tx)

/*------------------------------------- Global Variables ------------------------------------------*/ 

StaticJsonDocument<200> doc;

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
 
  // Configura WiFiClientSecure para usar as credenciais do dispositivo AWS IoT 
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);
 
  // Conecta ao broker MQTT no Endpoint da AWS
  client.setServer(AWS_IOT_ENDPOINT, 8883);
 
  // Cria o messege handler, objeto responsável por enviar as mensagens ao tópico cadastrado
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
 
  // Realiza a assinatura ao tópico especificado
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
 
  Serial.println("AWS IoT Connected!");
}

void publishMessage()
{
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); /* Escreve no Client */
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
  connectAWS(); /* Conecta a AWS */
  SerialPort.begin(115200, SERIAL_8N1, rxPin, txPin);
  Serial.begin(115200); // Apenas para ser visualizado no monitor do arduino, pode ser retirado
}

/*-----------------------------------Loop---------------------------------------------*/

void loop(void)
{

  if (SerialPort.available()) 
    {
    Serial.print("Entrou no IF");
    String jsonBuffer = SerialPort.readString();
    deserializeJson(doc, jsonBuffer);
    //String msg = SerialPort.readString();
    //Serial.print(msg); // Apenas para ser visualizado no monitor do arduino, pode ser retirado
    Serial.print(jsonBuffer);
    }
  //publishMessage();
  //client.loop();
  delay(5000);
}
