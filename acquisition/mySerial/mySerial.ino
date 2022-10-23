#include <SoftwareSerial.h>

#define rxPin 10
#define txPin 11

String c = "{\"env_temperature\":23.4,\"humidity\":68.9,\"water_temperature\":21.6,\"ec_value\":235,\"water_level\":1}";

// Set up a new SoftwareSerial object
SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);

void setup()  {
    // Define pin modes for TX and RX
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
    
    // Set the baud rate for the SoftwareSerial object
    mySerial.begin(115200);
}

void loop() 
{
 mySerial.print(c);
 delay(2000);
}
