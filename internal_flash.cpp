#include "internal_flash.h"

//defining structure variable here
EEPROM_Uni running_var;

/*
      @brief  This function sets the ROM portion to be used as EEPROM_X[PORT_1].
*/
void EEPROM_OPERATION::begin(void) {
  //initialize page address
  EEPROM.PageBase0 = 0x801D000;    // Base Address for Page 0...
  EEPROM.PageBase1 = 0x801D800;    // Base Address for Page 1...
  EEPROM.PageSize  = 0x400;        // ROM portion dedicated to be used as EEPROM...
  EEPROM.Status = EEPROM.init();           //initialize eeprom
}

/**
    @brief  Erases PAGE0 and PAGE1 and writes custom structure on the eeprom
    @param  custom structure
    @retval NULL
*/

//        void reset_default(void *obj, uint16_t Objsize) {
//
//        }

/**
    @brief  load structure data to internal flash
    @param  struct instance address, struct instance siz
    @retval NULL
*/

void EEPROM_OPERATION::set_struct_var(void *obj, uint16_t Objsize) {
  //format the pages to write new variable
  EEPROM.Status = EEPROM.format();

  //casting void object pointer to unsigned integer pointer
  uint16_t *ptr = (uint16_t*)obj;

  //checking for checksum
  uint16_t checksum = CalcCheckSum(&ptr, Objsize); 
  for (uint16_t i = 0; i < Objsize; i++)
    EEPROM.write (i, *(ptr++));
  EEPROM.write (Objsize + 1, checksum); //writing checksum at last address

}



/**
    @brief  read from eeprom and update the structure value if checksum is true
    @param  struct instance address, struct instance size
    @retval -1 if checksum is not matched else 1 on EEPROM_OK
*/
int16_t EEPROM_OPERATION::get_struct_var(void *obj, uint16_t Objsize) {

  //read whole structure and update
  uint16_t *ptr = (uint16_t*)obj;
  for (uint16_t i = 0; i < Objsize; i++)
    EEPROM.read(i, ptr++);

  //checking for checksum
  uint16_t checksum = CalcCheckSum(&ptr, Objsize); 
  
  //fetch flash stored checksum
  uint16_t str_checksum ;
  EEPROM.read(Objsize + 1, &str_checksum); 
  
  if (str_checksum == checksum) {
    return  CHECKSUM_OK;
  } else {
    return  CHECKSUM_ERROR;
  }

}


/**
    @brief  read structure from the eeprom if checksum is OK then load the current structure with eeprom value or reset with default
    @param
    @retval
*/
int16_t EEPROM_OPERATION::update_struct_var() {
  //initialize value with one instance


  //get eeprom value to another instance for the checking
  int8_t read_status = get_struct_var(&running_var, sizeof(running_var) / 2);
  Serial.print("read_status:"); Serial.println(read_status);
  
//  if (read_status == CHECKSUM_ERROR) {
    //reset with default values 
//    strcpy(running_var.Union.Mqtt.Topic.CompanyNm, "vectra"); //limit 15 char
//    strcpy(running_var.Union.Mqtt.Topic.DepartNm, "emb");  // limit 15 char
//    strcpy(running_var.Union.Mqtt.Topic.DeviceID, "brigde"); // limit 10 char
//    strcpy(running_var.Union.Mqtt.Topic.DeviceNo, "01"); //limit 5 char
//    strcpy(running_var.Union.Mqtt.Broker.server, "192.168.120.164"); //limit 15 char
//    running_var.Union.Mqtt.Broker.port = 1883; //limit 5 char
//    
//    set_struct_var(&running_var, sizeof(running_var) / 2);
//  }

//  if(read_status == CHECKSUM_OK){
    //show the stored value 
    Serial.print("eeprom.Union.CompanyNm:");      Serial.println( running_var.Union.Mqtt.Topic.CompanyNm );
    Serial.print("eeprom.Union.DepartNm:"); Serial.println( running_var.Union.Mqtt.Topic.DepartNm );
    Serial.print("eeprom.Union.DeviceID:");     Serial.println( running_var.Union.Mqtt.Topic.DeviceID );
    Serial.print("eeprom.Union.DeviceNo:");    Serial.println( running_var.Union.Mqtt.Topic.DeviceNo );
    Serial.print("running_var.Union.Mqtt.Broker.server:");     Serial.println( running_var.Union.Mqtt.Broker.server );
    Serial.print("running_var.Union.Mqtt.Broker.por:");    Serial.println( running_var.Union.Mqtt.Broker.port );
//  }
   
}



/**
    @brief  read object internal byte and apply xor operation
    @param  struct instance address, struct instance size
    @retval checksum with xor operation
*/

uint16_t EEPROM_OPERATION::CalcCheckSum(void *obj, uint16_t Objsize) {
  uint16_t *ptr = (uint16_t*)obj;
  uint16_t checksum = 0x0;

  for (uint16_t i = 0; i < Objsize; i++) {
    checksum ^= *(ptr++);
  }
  return checksum;
}
