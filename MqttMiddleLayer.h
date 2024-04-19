#ifndef MqttMiddleLayer_h
#define MqttMiddleLayer_h
 
#include <Ethernet_STM.h>
#include <PubSubClient.h>    
 
//----------------Serial--------------------- 
#define DEBUGOUT  //uncomment to turn off the debug massage

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

class Mqtt_client : public PubSubClient{ 
    private:
      unsigned long current_tm   = 0;
      unsigned long Reconnect_tm =  5000; //5 seconds
      unsigned long previous_tm  = -Reconnect_tm; //attempt on first success 
      String Sub_Topic       = "vectra/embedded/bridge/1";      //default
      String Pub_Topic_OUT   = Sub_Topic + "/out";              //default
      String Pub_Topic_BIRTH = Sub_Topic + "/birth";            //default
      String Pub_Topic_LW    = Sub_Topic + "/lwt" ;             //default 
      String Client_Name     = "bridge_xx";                     //default
      String will_Message    = "{" + Client_Name + ":\"disconnect\" }";
      String  mqtt_server = "192.168.120.164";//default port address 
      uint16_t mqtt_port = 1883; //default port address 
      void (*DHCP)() ;
      byte *mac;
      byte _RESETPIN;
      
    public: 
      
      Mqtt_client(Client& client );
      void begin(void (*callback)(char*, uint8_t*, unsigned int),void (*DHCP)(),byte *mac,byte RESETPIN);
      void hard_reset_wiz5500();
      void setDHCP(void (*DHCP)() ) ; 
      void setClientName(); 
      void setTopic(String CompanyNm,String DepartNm, String DeviceID,String DeviceNo);
      void setBroker(String server,uint16_t port);
      void viewTopic(); 
      String getSubTopic();
      String getPubTopic();
      void reconnect() ;
    
 };

 
#endif
