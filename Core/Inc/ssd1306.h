
#ifndef SSD1306_H
#define SSD1306_H

void lcd_move(uint8_t x,uint8_t y);
void lcd_init(I2C_HandleTypeDef c);
void lcd_printc(char chr,char cursor);
void lcd_printmap(char map[64][16]);
#endif