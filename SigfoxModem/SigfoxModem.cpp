/*
 * File : SigfoxModem.cpp
 *
 * Version : 1.2.1
 *
 * Purpose : Sigfox modem interface library for Arduino
 *
 * Author : Previmeteo (www.previmeteo.com)
 *
 * Project web site : http://oses.previmeteo.com/
 *
 * License: GNU GPL v2 (see License.txt)
 *
 * Creation date : 2014/10/07
 * 
 * History :
 *
 * 1.2.1 : addition of serialConnection.listen() command in the requestAT() method
 * 
 */
 
 
 
#include "Arduino.h"

#include "SoftwareSerial.h"

#include "SigfoxModem.h"





SigfoxModem::SigfoxModem(byte rxPin, byte txPin):serialConnection(rxPin, txPin) { 
    

  // rxPin : the pseudo-RX pin of the Arduino linked to the TXD pin of the Sigfox modem
  // txPin : the pseudo-TX pin of the Arduino linked to the RXD pin of the Sigfox modem
    
  _debugSerialConnectionEnabled = false;
  
}



SigfoxModem::SigfoxModem(byte rxPin, byte txPin, SoftwareSerial *debugSerialConnection):serialConnection(rxPin, txPin) { 
    
  // rxPin : the pseudo-RX pin of the Arduino linked to the TXD pin of the Sigfox modem
  // txPin : the pseudo-TX pin of the Arduino linked to the RXD pin of the Sigfox modem
  
  _debugSerialConnectionEnabled = true;
  
  _debugSerialConnection = debugSerialConnection;
  
}



void SigfoxModem::init() {
  
  serialConnection.begin(9600);
  serialConnection.listen();
  
  delay(1000);
  
  requestAT(F("ATE0"), 2, AT_DEFAULT_RESP_TIMOUT_IN_MS);  
  
}



boolean SigfoxModem::isAlive() {
  
  boolean isAlive = false;
  
  requestAT(F("AT"), 2, AT_DEFAULT_RESP_TIMOUT_IN_MS);

  if(strstr(_atRxBuffer, "OK") != NULL) isAlive = true;  
 
  return isAlive;
  
}


    
void SigfoxModem::retrieveSigfoxSerialNumber(char sigfoxSerialNumberBuffer[SIGFOX_SERIAL_NUMBER_BUFFER_SIZE]) {

  requestAT(F("ATI7"), 2, AT_DEFAULT_RESP_TIMOUT_IN_MS);  
  
  retrieveLineFromATRxBuffer(1, sigfoxSerialNumberBuffer, SIGFOX_SERIAL_NUMBER_BUFFER_SIZE);

}



int SigfoxModem::getDeviceTemperature() {

  char lineBuffer[13];

  requestAT(F("ATI26"), 2, AT_DEFAULT_RESP_TIMOUT_IN_MS);
  
  retrieveLineFromATRxBuffer(1, lineBuffer, sizeof(lineBuffer));
  
  int deviceTemperature = atoi(lineBuffer);
  
  return deviceTemperature;

}


   
boolean SigfoxModem::sendMessage(char message[SIGFOX_MESSAGE_BUFFER_SIZE]) {


  boolean messageSent = false;
  
  byte reqBufferLength = SIGFOX_MESSAGE_BUFFER_SIZE + 6;
  
  char reqBuffer[reqBufferLength];
  
  strcpy(reqBuffer, "AT$ss=");
  strcat(reqBuffer, message);

  requestAT(reqBuffer, 2, AT_SEND_MESSAGE_TIMOUT_IN_MS);
  
  if(strstr(_atRxBuffer, "OK") != NULL) messageSent = true;  
  
  return messageSent;
  
}



void SigfoxModem::requestAT(char *command, byte respMaxlinesCounter, long timeOutInMS) {
  
  
  if(DEBUG_MODE and _debugSerialConnectionEnabled) {
  
    _debugSerialConnection->print("-> ");
    _debugSerialConnection->println(command);
  
  }
  

  byte numOfCharsReceived = 0;
  byte linesCounter = 0;
    
  long clockTimeOut = millis() + timeOutInMS;
  
  serialConnection.print(command); 
  
  delay(200);
  
  serialConnection.flush();
  
  serialConnection.print("\n"); 
  
  serialConnection.listen();
  

  while ((millis() < clockTimeOut) && (numOfCharsReceived < (AT_RX_BUFFER_SIZE - 1)) && (linesCounter < respMaxlinesCounter)) {  
    
    if(serialConnection.available() > 0) {
       
      char c = serialConnection.read();

      _atRxBuffer[numOfCharsReceived] = c;
      numOfCharsReceived++;
       
      if(c=='\n')  linesCounter += 1;
       
    }
    
  }
  
  _atRxBuffer[numOfCharsReceived] = '\0';
  
  delay(200);

  serialConnection.flush();
  
  
  if(DEBUG_MODE and _debugSerialConnectionEnabled) {
  
    _debugSerialConnection->println(_atRxBuffer);
  
  }
  
  
}



void SigfoxModem::requestAT(const __FlashStringHelper *commandF, byte respMaxlinesCounter, long timeOutInMS) {

  byte commandLength = 0;
  
  prog_char *commandFPtr = (prog_char*) commandF;

  while(pgm_read_byte_near( commandFPtr + commandLength ) != '\0' ) commandLength++;
  
  char command[commandLength+1];
  
  memcpy_P(command, commandF, (commandLength + 1));
  
  command[commandLength] = '\0';
  
  requestAT(command, respMaxlinesCounter, timeOutInMS);
  
}



void SigfoxModem::retrieveLineFromATRxBuffer(byte lineIndex, char* lineBuffer, byte lineBufferLength) {

  byte atRxBufferLength = 0;

  while(_atRxBuffer[atRxBufferLength] != '\0') atRxBufferLength++;

  boolean lineFound = false;

  byte linesCounter = 0;
  
  byte nDelStartIndex = 0;
  byte nDelEndIndex = 0;
  
  char c;

  for(byte i = 0 ; ((i < atRxBufferLength) and !lineFound) ; i++) {
  
    c = _atRxBuffer[i];
    
    if(c == '\n') {
    
      nDelEndIndex = i;
      
      if(lineIndex == linesCounter) lineFound = true;
      
      else {
      
        nDelStartIndex = nDelEndIndex;
    
        linesCounter++;
      
      }
    
    }
  
  }  
  
  if(lineFound) {
  
    byte lineStartIndex;
    
    byte lineEndIndex;
    
    if(lineIndex == 0) lineStartIndex = 0;
    else lineStartIndex = nDelStartIndex + 1;
     
    if(lineIndex == 0 and nDelEndIndex < 2) lineEndIndex = 0;
    else lineEndIndex = nDelEndIndex - 2;    

    byte lineEndIndexCut = lineEndIndex;
    
    if((lineEndIndex - lineStartIndex) >= (lineBufferLength - 1)) lineEndIndexCut = lineStartIndex + lineBufferLength - 2;
    
        
    for(byte i = lineStartIndex ; i <= lineEndIndexCut ; i++) {
      
      lineBuffer[(i - lineStartIndex)] = _atRxBuffer[i];
      
    }
      
    
    lineBuffer[lineEndIndexCut - lineStartIndex + 1] = '\0'; 

  }
  
  else lineBuffer[0] = '\0';
  

}


