# Estufa hidropônica de precisão: Modelagem de sistema não linear utilizando Controle PID e Machine Learning 

Esse projeto de pesquisa tem como objetivo implementar um sistema de controle de parâmetros de análises físico-químicas que possibilite o melhor manejo de um modelo de agricultura de precisão num ambiente isolado (estufa hidropônica). O seguinte trabalho utilizará o controle de múltiplas variáveis, incluindo temperatura e umidade, luminosidade, condutividade, PH, Oxigênio, Dióxido de Carbono e Imagem. Isso será possível por meio da modelagem multivariável utilizando sensores e atuadores controlados via PID e otimizados via Redes Neurais. 

A opção pelo isolamento visa evitar o uso de defensivos agrícolas e estudar os efeitos do cultivo promovido via iluminação artificial, enquanto a opção pelo método hidropônico ocorreu para uma melhor precisão nos processos nutricionais do cultivo como o controle de condutividade, PH e oxigenação da água. O projeto tem ainda como objetivo elaborar um modelo que possa ser escalável e economicamente viável no que tange a eficiência produtiva, hídrica e energética, de modo a oferecer uma alternativa aos métodos convencionais de cultivo.

Os dados são enviados para o broker MQTT da Amazon Web Services (IOT-Core) via TCP/IP e formatados como Json. É a partir de uma estratégia Cloud que serão desenvolvidos as análises, atuações e recalibrações do sistema. A escolha por essa alternativa se deu pelo seu baixo custo e pela escalabilidade oferecida pelos serviços Cloud based.

## Bibliotecas Utilizadas

O seguinte projeto utiliza o microcontrolador ESP-32 e necessita das seguintes bibliotecas para o seu funcionamento

```<OneWire.h>``` ```<DallasTemperature.h>```

```<GravityTDS.h>``` ```<EEPROM.h>```

```<DHT.h>``` ```<WiFiClientSecure.h>```

```<PubSubClient.h>``` ```<ArduinoJson.h>```

```<WiFi.h>```
