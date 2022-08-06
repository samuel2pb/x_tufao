
/*inclusão das bibliotecas necessárias*/
#include <Arduino.h>
#include <SimpleDHT.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define pinDHT22 12          /* o pino de dados do sensor de temperatura ambiente DHT22 */
SimpleDHT22 dht22(pinDHT22); /* objeto temperatura e umidade da biblioteca SimpleDHT */

const char *ssid = "Samuel";
const char *password = "EAEAB9C85D";

// Add your MQTT Broker IP address, example:
// const char* mqtt_server = "192.168.1.144";
const char *mqtt_server = "192.168.0.28"; //hostname



WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

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

void setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
  //WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *message, unsigned int length)
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off".
  // Changes the output state according to the message
  if (String(topic) == "esp32/output")
  {
    Serial.print("Changing output to ");
    if (messageTemp == "on")
    {
      Serial.println("on");
      // digitalWrite(ledPin, HIGH);
    }
    else if (messageTemp == "off")
    {
      Serial.println("off");
      // digitalWrite(ledPin, LOW);
    }
  }
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client"))
    {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup(void)
{
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // pinMode(ledPin, OUTPUT);
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000)
  {
    lastMsg = now;

    float temperature, humidity;

    // Temperature in Celsius
    temperature = read_env_temp();

    // Convert the value to a char array
    char tempString[8];
    dtostrf(temperature, 1, 2, tempString);
    Serial.print("Temperature: ");
    Serial.println(tempString);
    client.publish("esp32/temperature", tempString);

    humidity = read_env_humidity();

    // Convert the value to a char array
    char humString[8];
    dtostrf(humidity, 1, 2, humString);
    Serial.print("Humidity: ");
    Serial.println(humString);
    client.publish("esp32/humidity", humString);
  }
}
