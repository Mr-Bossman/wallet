
#ifndef SSD1306_H
#define SSD1306_H
#include <stdio.h>

void lcd_move(uint8_t x,uint8_t y);
void lcd_init(I2C_HandleTypeDef c);
void lcd_printc(const char chr,const size_t cursor);
void lcd_printmap(const char map[64][16]);
void lcd_printcmap(char chr[8],const size_t cursor);
void lcd_printstr(const char *chr,size_t cursor, size_t offset);
void lcd_printlc(const char chr,const size_t cursor,size_t line);
void lcd_clear();
#endif