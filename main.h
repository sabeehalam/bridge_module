#ifndef _MAIN_H_
#define _MAIN_H_
#include "Arduino.h"
#include "conf.h" 

#ifdef  USE_INTERNAL_FLASH
#include "internal_flash.h"
 // initialize internal EEPROM instance
  EEPROM_OPERATION I_EEPROM;
#endif//USE_INTERNAL_FLASH

//-----------------------------------------
#ifdef  USE_ETHERNET
  #include <SPI.h> 
//  #ifdef  USE_SPI2  // in this module it's set by core library
//  SPIClass SPI(2);  
//  #endif//USE_SPI2
  #include <Ethernet_STM.h>

  //----------------------
  #ifdef CONFIG_PAGE_ENABLE
    #include "webserver.h"
    clienthandler webserver;
    EthernetServer local_server(7104); //server port
  #endif//CONFIG_PAGE_ENABLE
  
#endif//USE_ETHERNET


#ifdef  MQTT_ENABLE
  #include <PubSubClient.h> 
  #include <ArduinoJson.h>
  #include "MqttMiddleLayer.h"

  EthernetClient ethClient;
  Mqtt_client mqttclient(ethClient);

  #define RESETPIN PB1
  byte MAC[] = {0xDE, 0xAD, 0xBE, 0x88, 0x2b, 0x14};
  
#endif//MQTT_ENABLE  


//-----------------------------------------
#ifdef  WATCHDOG_ENABLE
  #include <libmaple/iwdg.h>
#endif//WATCHDOG_ENABLE

//-----------------------------------------
#ifdef  SERIALMODULE
 #include "SerialHandling.h" 
 #define SERIALBAUD 9600
 SerialHandling serialtalk(&mqttclient);
#endif//SERIALMODULE
#endif//_MAIN_H_
