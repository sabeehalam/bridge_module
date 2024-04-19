#include "SerialHandling.h"


SerialHandling::SerialHandling()
{ 
  _mqttclientimage = NULL;
}

SerialHandling::SerialHandling(Mqtt_client *mqttclientimage)
{ 
  _mqttclientimage = mqttclientimage;
}
/*Set the ModSerial Baud rate.
Reconfigure the UART for 8 data bits, no parity, and 1 stop bit.
and flush the ModSerial port.
*/
void SerialHandling::setBaud(word baud)
{
  _baud = baud;
  //calculate the time period for 3 characters for the given bps in ms.
  _frameDelay = 24000/_baud;

  _Serial.begin(baud); //configuration outside the class

  _Serial.flush();
}

///*
//Retrieve the ModSerial baud rate
//*/
word SerialHandling::getBaud(void)
{
  return(_baud);
}
//
///*
//  Checks the UART for query data
//*/
void SerialHandling::checkSerial(void)
{
  //while there is more data in the UART than when last checked
  while(_Serial.available()> _len)
  {
    //update the incoming query message length
    _len = _Serial.available();
    //Wait for 3 bytewidths of data (SOM/EOM)
    //delayMicroseconds(RTUFRAMETIME);
    delay(_frameDelay);
    //Check the UART again
  }
}

/*
Copies the contents of the UART to a buffer
*/
void SerialHandling::serialRx(void)
{
  byte i;

  //allocate memory for the incoming query message
  _msg = (byte*) malloc(_len);

    //copy the query byte for byte to the new buffer
    for (i=0 ; i < _len ; i++)
      _msg[i] = _Serial.read();
}

//serial transmittion from the device
void SerialHandling:: serialTx(byte* payload , unsigned int length){
//  _Serial.println("Recived from cloud");

  _Serial.write(payload,length);  //testing phase 
}


void SerialHandling::run(void)
{
 
  int i;
  
  //initialize mesasge length
  _len = 0;

  //check for data in the recieve buffer
  this->checkSerial();

  //if there is nothing in the recieve buffer, bail.
  if(_len == 0)
  {
    return;
  }

  //retrieve the query message from the Serial uart
  this->serialRx();
  
  //send to cloud 
  _mqttclientimage->publish(_mqttclientimage->getPubTopic().c_str(),_msg,_len);
// for debugging
//  Serial.print("Recieved data:");
//  for(i = 0 ; i < _len ; i++){
//     Serial.print(_msg[i],HEX); 
//  }
  
   //free the allocated memory for the reply message
   free(_msg);
   //reset the message length
   _len = 0;
  
//  //if a reply was generated
//  if(_len)
//  {
//    int i;
//    //send the reply to the Serial UART
//    //Senguino doesn't support a bulk Serial write command....
//    for(i = 0 ; i < _len ; i++)
//      ModSerial.write(_msg[i]);
//    //free the allocated memory for the reply message
//    free(_msg);
//    //reset the message length
//    _len = 0;
//  }
}
