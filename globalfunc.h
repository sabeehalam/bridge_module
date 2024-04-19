// GLOBAL FUNCTION 
 
//---------------callback function to catch mqttt data--------
void callback(char* topic, byte* payload, unsigned int length) {
  //send mqtt recived message to serial
  serialtalk.serialTx(payload, length);
}

//---------------connect to network with Dynamic_IP--------
void ConnectWithDHCP() {
  // start the Ethernet connection:
  DEBUG_PRINTLN("Initialize Ethernet with DHCP with unique MAC:");

  //  if (!restart_count) { //only once at restart

  //update new mac address from unique_ID of stm32f103
  //generating mac address from controller UNIQUI_ID
#define  U_ID         0x1ffff7e8      //unique id pointer address
  for (int i = 2; i >= 0; i--) {
    MAC[5 - i] = (*(volatile byte *)(U_ID + i));
    MAC[2 - i] = (*(volatile byte *)(U_ID + i + 9));
  }
  // for Custom MAC i.e. "Locally Administered Addresses", second LSB should be HIGh and
  //first LSB should be LOW (Unicast transmission)
  MAC[0] = 0xFE;


  if (Ethernet.begin(MAC) == 0) {
    DEBUG_PRINTLN("Failed to configuration");
    byte static_ip[] = {192, 168, 0, 144}; //local ip On LAN
    Ethernet.begin(MAC, static_ip);
    DEBUG_PRINT("static ip address: ");
    DEBUG_PRINTLN(Ethernet.localIP());
  }
  // print your local IP address:
  DEBUG_PRINT("My IP address: ");
  DEBUG_PRINTLN(Ethernet.localIP());
}
