#define EEPROM_BASE_ADDR 0x08000000UL
#define EEPROM_SIZE FLASH_SIZE
#define EEPROM_PAGE_SIZE 0x00001000UL
#define EEPROM_USE_OFFSET 0x60000
#include <stdio.h>
#include <string.h>
#include "eeprom.h"

static uint32_t roundto(uint32_t r, uint32_t v)
{
	return (v / r) * r;
}

static uint32_t FLASH_READ_64(uint32_t offset, uint64_t *buf, size_t len)
{
	if (offset + len > EEPROM_SIZE)
		return -1;
	if (len > EEPROM_PAGE_SIZE)
		return -2;
	intptr_t tmp = EEPROM_BASE_ADDR + offset;
	__IO uint64_t *addr = (__IO uint64_t *)(tmp);
	for (size_t i = 0; i < len; i++)
		buf[i] = addr[i];
	return 0;
}

// offset in bytes from start of flash
uint32_t FLASH_READ(uint32_t off, uint8_t *buf, size_t len)
{
	if (off + len > EEPROM_SIZE)
		return -1;
	if (len > EEPROM_PAGE_SIZE)
		return -2;
	intptr_t start = roundto(8, EEPROM_BASE_ADDR + off);
	__IO uint64_t *addr = (__IO uint64_t *)(start);
	uint64_t tmp = 0;
	size_t i = 0;
	uint32_t offset = off;
	if (len + (offset % 8) <= 8)
	{
		tmp = addr[0];
		memcpy(buf, ((char *)&tmp) + (offset % 8), len);
	}
	else
	{
		size_t copiable_size = (len + (offset % 8)) / 8;
		for (; i < copiable_size; i++)
		{
			tmp = addr[i];
			memcpy(buf, ((char *)&tmp) + (offset % 8), 8 - (offset % 8));
			buf += 8 - (offset % 8);
			offset = 0;
		}
		tmp = addr[i];
		memcpy(buf, &tmp, (len + off) % 8);
	}
	return 0;
}

static uint32_t clear_page(uint64_t save[EEPROM_PAGE_SIZE], uint64_t addr)
{

	uint32_t pg = roundto(EEPROM_PAGE_SIZE, addr - EEPROM_BASE_ADDR);
	uint32_t ret = 0;
	if (save)
		ret = FLASH_READ_64(pg, save, EEPROM_PAGE_SIZE);
	if (ret)
		return ret;
	FLASH_EraseInitTypeDef er = {
		.TypeErase = FLASH_TYPEERASE_PAGES,
		.Page = (pg / EEPROM_PAGE_SIZE),
		.NbPages = 1};
	HAL_FLASH_Unlock();
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
	HAL_FLASHEx_Erase(&er, &ret);
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
	HAL_FLASH_Lock();
	return ret; // idk
}

// offset in bytes from start of flash
uint32_t FLASH_WRITE(uint32_t offset, uint8_t *dat, size_t len)
{
	uint32_t status = 0;
	if (offset + len > EEPROM_SIZE)
		return -1;
	if (len > EEPROM_PAGE_SIZE)
		return -2;
	uint32_t a = EEPROM_BASE_ADDR + offset;
	uint32_t pg = roundto(EEPROM_PAGE_SIZE, a);
	uint64_t *save = malloc(EEPROM_PAGE_SIZE);
	clear_page(save, pg);
	memcpy(((uint8_t *)save) + (offset % EEPROM_PAGE_SIZE), dat, len);
	status = HAL_FLASH_GetError();
	HAL_FLASH_Unlock();
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
	for (size_t i = 0; i < EEPROM_PAGE_SIZE / 8; i++)
	{
		status |= HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, pg + (i * 8), save[i]);
		if (status)
			break;
	}
	free(save);
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
	status = HAL_FLASH_GetError();
	HAL_FLASH_Lock();
	return status;
}

//starts at one zero is empty
uint16_t FLASH_LEN_DATA()
{
	uint16_t i = 0;
	uint32_t off = EEPROM_USE_OFFSET;
	uint16_t tmpof = 0;
	while (1)
	{
		FLASH_READ(off, (uint8_t *)&tmpof, 2);
		off += tmpof;
		if (tmpof == UINT16_MAX || tmpof == 0 || tmpof >= EEPROM_SIZE)
			break;
		i++;
	}
	return i;
}

//len in bytes, offset in writes
uint16_t FLASH_READ_DATA(uint16_t offset, uint8_t *buf, uint16_t len)
{
	uint16_t count = FLASH_LEN_DATA();
	if (offset >= count)
		return count;
	uint32_t off = EEPROM_USE_OFFSET;
	uint16_t tmpof = 0;
	while (offset--)
		FLASH_READ(off, (uint8_t *)&tmpof, 2), off += tmpof;
	FLASH_READ(off, (uint8_t *)&tmpof, 2);
	if ((tmpof - 2) < len)
		return -4;
	return FLASH_READ(off + 2, buf, len);
}

// need to check overwrite size
//len in bytes, offset in writes
uint16_t FLASH_WRITE_DATA(uint16_t offset, uint8_t *buf, uint16_t len)
{
	uint16_t count = FLASH_LEN_DATA();
	if (offset > count)
		return count;
	uint32_t off = EEPROM_USE_OFFSET;
	uint16_t tmpof = 0;
	while (offset--)
		FLASH_READ(off, (uint8_t *)&tmpof, 4), off += tmpof;
	len += 2;
	uint32_t ret = FLASH_WRITE(off, (uint8_t *)&len, 2);
	len -= 2;
	if (ret)
		return ret;
	ret = FLASH_WRITE(off + 2, buf, len);
	return ret;
}
