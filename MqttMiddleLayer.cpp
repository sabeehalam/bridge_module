#include "MqttMiddleLayer.h"


Mqtt_client::Mqtt_client(Client& client) {
  setClient(client);
}

void Mqtt_client:: begin(void (*callback)(char*, uint8_t*, unsigned int), void (*DHCP)(),byte *mac,byte RESETPIN) {
  setServer(mqtt_server.c_str(), mqtt_port);
  setCallback(callback);
  setDHCP(DHCP);
  this->mac = mac;
  this->_RESETPIN = RESETPIN;
  hard_reset_wiz5500(); //reset wiznet
  setClientName();
  DHCP(); //start ethernet
}

void Mqtt_client::hard_reset_wiz5500() {
  //reset wiznet5500
  pinMode(_RESETPIN,OUTPUT);
  digitalWrite(_RESETPIN, LOW);
  delay(500);  //500us delay
  digitalWrite(_RESETPIN, HIGH);
  delay(500);   //1ms delay
}

//allow to access dhcp function from external location
void  Mqtt_client::setDHCP(void (*DHCP)() ) {
  this->DHCP = DHCP;
}

//setting client name based on mac address
void  Mqtt_client::setClientName() {

  //using these to overcome the starting zero issue e.g 0x0d appear as d only
  String f[3];
  for (int i = 0 ; i < 3 ; i++) {
    mac[i + 3] <= 0x0F ?  f[i] = 0 + String(mac[i + 3], HEX) :  f[i] =  String(mac[i + 3], HEX) ;
  }
  
  //print last three digit of mac in client name
  Client_Name = HOST_NAME  + f[0] + f[1] + f[2];
  //update will message
  will_Message    = "{" + Client_Name + ":\"disconnect\" }";
  DEBUG_PRINT("Client_Name:"); DEBUG_PRINTLN(Client_Name);
}

//making dynamic topic
void Mqtt_client::setTopic(String CompanyNm="vectra", String DepartNm = "xx", String DeviceID = "xx", String DeviceNo = "01") {
  Sub_Topic  = CompanyNm + "/" ;
  Sub_Topic += DepartNm  + "/" ;
  Sub_Topic += DeviceID  + "/" ;
  Sub_Topic += DeviceNo  ;

  Pub_Topic_OUT      = Sub_Topic + "/out";  //topic for normal operation
  Pub_Topic_LW       = Sub_Topic + "/lwt";   //topic for LW
  Pub_Topic_BIRTH    = Sub_Topic + "/birth";//topic for Device startup ALERT

  String will_Message    = "{\"" + Client_Name + "\":\"disconnect\" }";

  viewTopic();
}

// to set the server and port 
void Mqtt_client::setBroker(String server,uint16_t port){
  mqtt_server = server;
  mqtt_port = port;
}

void Mqtt_client::viewTopic() {
  DEBUG_PRINTLN("------GENERATED TOPICS---");
  DEBUG_PRINT("Sub_Topic:"); DEBUG_PRINTLN(Sub_Topic);
  DEBUG_PRINT("Pub_Topic_OUT:"); DEBUG_PRINTLN(Pub_Topic_OUT);
  DEBUG_PRINT("Pub_Topic_LW:"); DEBUG_PRINTLN(Pub_Topic_LW);
  DEBUG_PRINT("Pub_Topic_BIRTH:"); DEBUG_PRINTLN(Pub_Topic_BIRTH);
}

//------getter--------------
String  Mqtt_client::getSubTopic(){
  return Sub_Topic;
}
String  Mqtt_client::getPubTopic(){
  return Pub_Topic_OUT;
}

/*
   Reconnect to mqtt while disconnect and update the blink status
   fast blink for ethernet(link) down
   slow blink for Mqtt broker lost
*/
void Mqtt_client::reconnect() {

  current_tm = millis();
  if (current_tm - previous_tm >= Reconnect_tm) { //attempt in 5 seconds.
    //----------attempt to physical connection---------------------------
    //donot attempt to connect when physically not connected
    if (bitRead(W5100.getPHYCFGR(), 0)) {
       DHCP();
       
       DEBUG_PRINT("Attempting MQTT connection...");
      //-----------attemp to mqtt-broker connection---------------------------
      if (connect(Client_Name.c_str(), Pub_Topic_LW.c_str(), 0, 0, will_Message.c_str())) {
        //                digitalWrite(Network_LED, true); //Turn off blinking

        DEBUG_PRINTLN("connected");

        String awake_message    = "{\"" + Client_Name + "\":\"connected\" }";
        PubSubClient::publish(Pub_Topic_BIRTH.c_str(), awake_message.c_str());
        PubSubClient::subscribe(Sub_Topic.c_str()); // ... and resubscribe
      } else {
        DEBUG_PRINT("failed, rc=");
        DEBUG_PRINTLN(state());
        DEBUG_PRINTLN("try again in 5 seconds");
        previous_tm = millis();
      }
    }
  }
}
