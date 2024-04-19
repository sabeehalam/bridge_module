#include "webserver.h"

clienthandler:: clienthandler(void) { //constructor
  _EEPROM_OPERATION_image = NULL;
}
clienthandler:: clienthandler(EEPROM_OPERATION *EEPROM_OPERATION_image) { //if you want to pass an class object
  _EEPROM_OPERATION_image = EEPROM_OPERATION_image;
}
void clienthandler:: process(EthernetClient client) {

  // if login session timeout change state to 1
  if (millis() - session_start > TIMEOUT) {
    state = 1;
  }

  if (client) {
    while (client.connected()) {
      if (client.available()) {
        //capture  http get-request
        String get_request = client.readStringUntil('\r');

        //check for login credential
        if (getFieldValue(get_request, "username") == "admin" &
            getFieldValue(get_request, "password") == "extens1tyAdm1n") {
          state = 2;
          session_start = millis();
        }

        if (state == 2) { //update only when login session open

          //use to response to DEFAULT command
          //update the variable of running variable with default parameter
          if (get_request.indexOf("RESET") > 1) {
            get_default_var();
          }

          //use to response to save command
          //update the variable of running variable
          if (get_request.indexOf("CONFIG") > 1) {
            update_eeprom(get_request);
            state = 1;
          } 
        }

        //now render HTML page according to state
        if (state == 1) {
          render_page_1(client); //call login_page
        }
        if (state == 2) {
          render_page_2(client); //call login_page
        }

        break;
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
}

void clienthandler::render_page_1(EthernetClient client) {
  client.println(F("HTTP/1.1 200 OK"));
  client.println(F("Content-Type: text/html"));
  client.println();
  client.println(F("<HTML>"));
  client.println(F("<HEAD>"));
  client.println(F("<TITLE>Extensity </TITLE>"));
  client.println(F("<style> body{width: 129%;text-align: center; margin: 100px 0 0 -164px; background : #8DA6C8}h1{color: #3b3b73;}</style>"));
  client.println(F("</HEAD>"));

  client.println(F("<BODY>"));
  client.println(F("<H1>EXTENSITY</H1>"));
  client.println(F("<H2>ASSEMBLY LINE MONITOR</H2>"));
  client.println(F("<FORM ACTION='/' method=get >"));
  client.println(F("<form action=\"/action_page.php\">"));
  client.println(F("<label for='username'>Username:</label><br>"));
  client.println(F("<input type='text' id='username' name='username' value=''><br>"));
  client.println(F("<label for='password'>Password:</label><br>"));
  client.println(F("<input type='password' id='password' name='password' value=''><br><br>"));
  client.println(F("<input type=\'submit' value='Submit'> "));
  client.println(F("</form>"));

  client.println("");
  client.println(F("</BODY>"));
}

void clienthandler::render_page_2(EthernetClient client) {
  const char *formEnd = "' SIZE='25' MAXLENGTH='50' ><br></td></tr>"; //to reduce the space

  client.println(F("HTTP/1.1 200 OK"));
  client.println(F("Content-Type: text/html"));
  client.println();
  client.println(F("<!DOCTYPE HTML>"));
  client.println(F("<html>"));
  client.println(F("<TITLE>Extensity </TITLE>"));
  client.println(F("<style> body{width: 129%;  margin: 100px 0 0 500px; background : #8DA6C8}h1{color: #3b3b73;}</style>"));
  client.println(F("</HEAD>"));

  client.println(F("<BODY>"));
  client.println(F("<H1>EXTENSITY</H1>"));
  client.println(F("<H2>ASSEMBLY LINE MONITOR</H2>"));

  client.println(F("<table class = 'centre'><FORM ACTION='/' method=get >")); //uses IP/port of web page

  client.print(F("<tr><td>MQTT SERVER</td><td><INPUT TYPE=TEXT NAME='MQTT_SERVER' VALUE='"));
  client.print(String(running_var.Union.Mqtt.Broker.server));
  client.println(formEnd);

  client.print(F("<tr><td>MQTT PORT</td><td><INPUT TYPE=TEXT NAME='MQTT_PORT' VALUE='"));
  client.print(String(running_var.Union.Mqtt.Broker.port));
  client.println(formEnd);

  client.print(F("<tr><td>COMPANY NAME</td> <td> <INPUT TYPE=TEXT NAME='CompanyNm' VALUE='"));
  client.print(String(running_var.Union.Mqtt.Topic.CompanyNm));
  client.println(formEnd);

  client.print(F("<tr><td>DEPARTMENT NAME</td><td><INPUT TYPE=TEXT NAME='DepartNm' VALUE='"));
  client.print(String(running_var.Union.Mqtt.Topic.DepartNm));
  client.println(formEnd);

  client.print(F("<tr><td>DEVICE ID</td><td><INPUT TYPE=TEXT NAME='DeviceID' VALUE='"));
  client.print(String(running_var.Union.Mqtt.Topic.DeviceID));
  client.println(formEnd);

  client.print(F("<tr><td>DEVICE NO</td><td><INPUT TYPE=TEXT NAME='DeviceNo' VALUE='"));
  client.print(String(running_var.Union.Mqtt.Topic.DeviceNo));
  client.println(formEnd);


  client.println(F("<tr><td><INPUT TYPE=SUBMIT NAME='RESET' VALUE='DEFAULT'> </td>"));

  client.println(F("<td> <INPUT TYPE=SUBMIT NAME='CONFIG' VALUE='SAVE'></td></tr>"));
  client.println(F("</FORM></table>"));
  client.println(F("</html>"));
}
void clienthandler::update_eeprom(String &request) {
   //update the ram value with get request fields
   strcpy(running_var.Union.Mqtt.Broker.server, getFieldValue(request,"MQTT_SERVER").c_str());
   running_var.Union.Mqtt.Broker.port = getFieldValue(request,"MQTT_PORT").toInt();
   strcpy(running_var.Union.Mqtt.Topic.CompanyNm, getFieldValue(request,"CompanyNm").c_str());
   strcpy(running_var.Union.Mqtt.Topic.DepartNm, getFieldValue(request,"DepartNm").c_str());
   strcpy(running_var.Union.Mqtt.Topic.DeviceID, getFieldValue(request,"DeviceID").c_str());
   strcpy(running_var.Union.Mqtt.Topic.DeviceNo , getFieldValue(request,"DeviceNo").c_str());

   //store to internal eeprom 
  _EEPROM_OPERATION_image->set_struct_var(&running_var, sizeof(running_var) / 2);
  _EEPROM_OPERATION_image->update_struct_var(); //just to debug
}

//use to update with default value
void clienthandler::get_default_var() {
  
  strcpy(running_var.Union.Mqtt.Topic.CompanyNm, "vectra"); //limit 15 char
  strcpy(running_var.Union.Mqtt.Topic.DepartNm, "emb");  // limit 15 char
  strcpy(running_var.Union.Mqtt.Topic.DeviceID, "brigde"); // limit 10 char
  strcpy(running_var.Union.Mqtt.Topic.DeviceNo, "01"); //limit 5 char
  strcpy(running_var.Union.Mqtt.Broker.server, "192.168.120.164"); //limit 15 char
  running_var.Union.Mqtt.Broker.port = 1883; //limit 5 char
}

String clienthandler::getFieldValue(String incoming, String fieldName) {
  int startIndex = incoming.indexOf(fieldName + "=") + fieldName.length() + 1;
  int endIndex = incoming.indexOf("&", startIndex);
  if (endIndex == -1) {
    endIndex = incoming.indexOf(" HTTP/", startIndex);
  }
  return incoming.substring(startIndex, endIndex);
}
