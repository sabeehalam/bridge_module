#include "main.h"
#include "globalfunc.h"

void setup() {

#ifdef  DEBUGOUT
 // while (!Serial);
  Serial.begin(DEBUG_BAUD);
#endif//DEBUGOUT 


#ifdef  WATCHDOG_ENABLE
  iwdg_init(IWDG_PRE_256, 3500); //watch-dog 5000*0.0064 = 32000 ms
#endif//WATCH_ENABLE   

#ifdef  USE_INTERNAL_FLASH
  I_EEPROM.begin();
  I_EEPROM.update_struct_var(); 
#endif//USE_INTERNAL_FLASH


#ifdef  MQTT_ENABLE
  mqttclient.begin(callback, ConnectWithDHCP, MAC, RESETPIN);
  mqttclient.setBroker(running_var.Union.Mqtt.Broker.server,running_var.Union.Mqtt.Broker.port );
  mqttclient.setTopic(running_var.Union.Mqtt.Topic.CompanyNm,
                      running_var.Union.Mqtt.Topic.DepartNm ,
                      running_var.Union.Mqtt.Topic.DeviceID ,
                      running_var.Union.Mqtt.Topic.DeviceNo); 
#endif//MQTT_ENABLE

#ifdef  SERIALMODULE
  serialtalk.setBaud(SERIALBAUD);
#endif//SERIALMODULE  

#ifdef  CONFIG_PAGE_ENABLE
  local_server.begin(); 
#endif//CONFIG_PAGE_ENABLE
}

void loop() {
  
//feeding to watchdog continuously to prevent from code strucking
#ifdef  WATCHDOG_ENABLE
  iwdg_feed(); //Feeding watch dog
#endif//WATCH_ENABLE    

//handle server request process
#ifdef CONFIG_PAGE_ENABLE
    //if tcp-client is there launch a webpage
    EthernetClient client = local_server.available(); 
    webserver.process(client); //      Timer2.resume(); 
#endif//CONFIG_PAGE_ENABLE 

//handle mqtt process
#ifdef  MQTT_ENABLE
  //--link and mqtt reconnection status -------------
  if (!mqttclient.connected()) {
    mqttclient.reconnect();
  }
  mqttclient.loop(); //mqttclient process handler
#endif//MQTT_ENABLE  

//handle serial process
#ifdef  SERIALMODULE
  //it will listen then incoming serial data
  serialtalk.run();
#endif//SERIALMODULE  
}
