#ifndef EEPROM_H
#define EEPROM_H
#include "cmsis_os.h"
#include <stdlib.h>
#include <stdint.h>
#include "main.h"
uint32_t FLASH_WRITE(uint32_t offset, uint8_t *dat, size_t len);
uint32_t FLASH_READ(uint32_t offset,uint8_t *buf,size_t len);
uint16_t FLASH_READ_DATA(uint16_t offset,uint8_t *buf,uint16_t len);
uint16_t FLASH_WRITE_DATA(uint16_t offset,uint8_t *buf,uint16_t len);
uint16_t FLASH_LEN_DATA();
#endif