#ifndef SERIALHANDLING_H
#define SERIALHANDLING_H

#include <Arduino.h>
#include "MqttMiddleLayer.h"
//Maximum serial wait in micro seconds
#define SERIALMAXDELAY  100
#define SERIALBAUD    9600 
#define _Serial Serial1 

class SerialHandling
{
  private:
    Mqtt_client *_mqttclientimage;
    void checkSerial(void);
    void serialRx(void);
  public:
    SerialHandling(void);
    SerialHandling(Mqtt_client *mqttclientimage); //if you want to pass an class object
    void setBaud(word);
    word getBaud(void);
    void serialTx(byte* payload , unsigned int length);
    void run(void);
  

  private:
    byte *_msg,
          _len;

    word _baud, 
       _frameDelay;
};

#endif//SERIALHANDLING_H
