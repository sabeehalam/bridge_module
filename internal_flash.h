#ifndef INTERNAL_FLASH_H
#define INTERNAL_FLASH_H

#include <Arduino.h> 
#include <EEPROM.h>  
#include "conf.h"

enum definition 
{  
  CHECKSUM_OK    =  1,
  CHECKSUM_ERROR = -1
};

class EEPROM_OPERATION {
  private:
    uint16_t CalcCheckSum(void *obj, uint16_t Objsize)  ;
  public: 
    void begin(void) ;   
    void set_struct_var(void *obj, uint16_t Objsize);
    int16_t get_struct_var(void *obj, uint16_t Objsize) ;
    int16_t update_struct_var();
    
};

#endif //INTERNAL_FLASH_H
