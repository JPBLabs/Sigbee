
#include "SoftwareSerial.h"

#include "SigfoxModem.h"



// pins definition

#define DEBUG_TX_PIN 3
#define DEBUG_RX_PIN 255

#define SIGFOX_MODEM_TX_PIN 6                          // the pseudo-TX pin of the Arduino linked to the DIN pin of the XBee socket <->  RXD pin of the Sigfox modem
#define SIGFOX_MODEM_RX_PIN 7                          // the pseudo-RX pin of the Arduino linked to the DOUT pin of the XBee socket  <->  TXD pin of the Sigfox modem


SoftwareSerial softSerialDebug(DEBUG_RX_PIN, DEBUG_TX_PIN); 

SigfoxModem modem(SIGFOX_MODEM_RX_PIN, SIGFOX_MODEM_TX_PIN, &softSerialDebug);

//SigfoxModem modem(SIGFOX_MODEM_RX_PIN, SIGFOX_MODEM_TX_PIN);









/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// setup() and loop() functions definition
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void setup()  {
  
  Serial.begin(9600);
  
  softSerialDebug.begin(9600);
  
  delay(2000);
  
  Serial.println("Go !!!");
  
  softSerialDebug.println("Go !!!");
    
  modem.init();
  
  char message[13] = "001122334455";
  
  boolean success = modem.sendMessage(message); 
  
  Serial.print("Message sent : ");
  Serial.println(success);
 
}
  




void loop() {
  
 char sigfoxSerialNumberBuffer[9];
 
 modem.retrieveSigfoxSerialNumber(sigfoxSerialNumberBuffer);
 
 Serial.print("SIGFOX SERIAL NUMBER : ");
 Serial.println(sigfoxSerialNumberBuffer);
 
 int deviceTemperature = modem.getDeviceTemperature();
 
 Serial.print("DEVICE TEMPERATURE : ");
 Serial.println(deviceTemperature);
 
 delay(5000);
  
}




