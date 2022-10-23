#define rxPin 16
#define txPin 17

#include <HardwareSerial.h>

HardwareSerial SerialPort(2); // Esta sendo utilizado a UART 2 (pinos 16 e 17 do esp rx tx)

void setup()  {
    SerialPort.begin(115200, SERIAL_8N1, rxPin, txPin);
    Serial.begin(115200); // Apenas para ser visualizado no monitor do arduino, pode ser retirado
}

void loop() {
    if (SerialPort.available()) 
    {
     String c = SerialPort.readString();
     Serial.print(c); // Apenas para ser visualizado no monitor do arduino, pode ser retirado
    }
}
