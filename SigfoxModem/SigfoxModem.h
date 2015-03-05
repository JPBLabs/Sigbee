/*
 * File : SigfoxModem.h
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
 
 

#ifndef SIGFOX_MODEM_h
#define SIGFOX_MODEM_h



#include "Arduino.h"

#include "SoftwareSerial.h"





#define AT_RX_BUFFER_SIZE 101

#define SIGFOX_SERIAL_NUMBER_BUFFER_SIZE 9

#define SIGFOX_MESSAGE_BUFFER_SIZE 25



# define DEBUG_MODE true


#define AT_DEFAULT_RESP_TIMOUT_IN_MS 2000

#define AT_SEND_MESSAGE_TIMOUT_IN_MS 7000






class SigfoxModem {


  public:

    SoftwareSerial serialConnection;
    
    SigfoxModem(byte rxPin, byte txPin);
    
    SigfoxModem(byte rxPin, byte txPin, SoftwareSerial *debugSerialConnection);
    
    void init();
        
    boolean isAlive();
    
    void retrieveSigfoxSerialNumber(char sigfoxSerialNumberBuffer[SIGFOX_SERIAL_NUMBER_BUFFER_SIZE]); 
    
    int getDeviceTemperature(); 
    
    boolean sendMessage(char message[SIGFOX_MESSAGE_BUFFER_SIZE]);
    
    

  private:
    
    char _atRxBuffer[AT_RX_BUFFER_SIZE];
    
    SoftwareSerial *_debugSerialConnection;
    
    boolean _debugSerialConnectionEnabled;
    
    void requestAT(char *command, byte respMaxNumOflines, long timeOutInMS);
    
    void requestAT(const __FlashStringHelper *commandF, byte respMaxNumOflines, long timeOutInMS);
    
    void retrieveLineFromATRxBuffer(byte lineIndex, char* lineBuffer, byte lineBufferLength);

};



#endif