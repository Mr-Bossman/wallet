
#ifndef SSD1306_H
#define SSD1306_H

void lcd_move(uint8_t x,uint8_t y);
void lcd_init(I2C_HandleTypeDef c);
void lcd_printc(const char chr,const char cursor);
void lcd_printmap(const char map[64][16]);
void lcd_printstr(const char *chr,char cursor);
void lcd_clear();
#endif