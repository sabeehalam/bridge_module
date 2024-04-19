#ifndef _CONF_H_
#define _CONF_H_

//-----MODULE-SETTING---- 
#define DEBUGOUT     //for serial debug printing
#define USE_INTERNAL_FLASH  //configuration parameter from internal flash
#define USE_ETHERNET    //spi2 is using from library spi.c and 5500.c rogarclark
#define USE_SPI2     //uncomment will switch to spi_1[note use only with Ethernet_STM.h buildin RogarClark improved Spi.c ]
#define MQTT_ENABLE  
#define WATCHDOG_ENABLE
#define SERIALMODULE
#define CONFIG_PAGE_ENABLE

//----------------EEPROM_CUSTUM_STRUCTURE --------------------- 
#ifdef USE_INTERNAL_FLASH  


#define maxServerLen     16 
#define maxCompanyLen    16 
#define maxDepartLen     14 
#define maxDeviceIDLen   11 
#define maxDeviceNoLen   5 

struct MQTT_Broker {
  uint16_t port; 
  char server[maxServerLen];  //limit to maxServerLen character 
};

struct MQTT_Topic { 
  char CompanyNm[maxCompanyLen];   //limit to maxCompanyLen character
  char DepartNm[maxDepartLen];     //limit to maxDepartLen character 
  char DeviceID[maxDeviceIDLen];   //limit to maxDeviceIDLen character
  char DeviceNo[maxDeviceNoLen];   //limit to maxDeviceNoLen character
};

struct MQTT_Configurations {
  MQTT_Broker Broker;
  MQTT_Topic Topic;
};

struct EEPROM_Configurations {
  MQTT_Configurations Mqtt;  
};

union EEPROM_Uni {
  //don't edit this union structure 
  //   struct EEPROM_Whole_Data Union;
  struct EEPROM_Configurations Union;
  uint16_t HalfWord[sizeof(EEPROM_Configurations) / 2]; // 16 bit is taken to use less storage  as the library utilises 2 Bytes of eeprom to store a single byte of data...
};


extern EEPROM_Uni running_var;
   
#endif //USE_INTERNAL_FLASH  



//----------------Serial--------------------- 
#define DEBUGOUT  //uncomment to turn off the debug massage
#define DEBUG_BAUD
#ifdef DEBUGOUT //Use USB_Serial for debug communication
#define DEBUG_PRINT_F(...)    Serial.print(F(__VA_ARGS__))
#define DEBUG_PRINTLN_F(...)  Serial.println(F(__VA_ARGS__))
#define DEBUG_PRINT(...)    Serial.print(__VA_ARGS__)
#define DEBUG_PRINTLN(...)  Serial.println(__VA_ARGS__) 
#else
#define DEBUG_PRINT_F(...) //else remove the debug  feature
#define DEBUG_PRINTLN_F(...)
#define DEBUG_PRINT(...)
#define DEBUG_PRINTLN(...)
#endif //DEBUGOUT  

#endif//_CONF_H_
