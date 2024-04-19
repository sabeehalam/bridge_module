
#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Ethernet_STM.h> 
#include "internal_flash.h"

#define CONV 60*1000
#define TIMEOUT 0.5*CONV  //5min time out to login sesssion


//inherit the property of ethernetclient
class clienthandler{
  private: 
   EEPROM_OPERATION *_EEPROM_OPERATION_image;
   int state = 1;  //use for switching the pages 
   unsigned long session_start = 1 ;

   void render_page_1(EthernetClient client);  
   void render_page_2(EthernetClient client);  
   String getFieldValue(String incoming, String fieldName);
   void update_eeprom(String &request);
   void get_default_var();
  public:
   clienthandler(void); //constructor
   clienthandler(EEPROM_OPERATION *EEPROM_OPERATION_image); //constructor with argument class object
   void process(EthernetClient client);
};
#endif//WEBSERVER_H
