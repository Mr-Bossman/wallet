#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "cmsis_os.h"
#include "main.h"

#include "font8x8.h"
#include "ssd1306.h"

const char init_[] = {0xAE,0x8D,0x14,0xAF};

static I2C_HandleTypeDef hi2c1;

void lcd_move(uint8_t x,uint8_t y){
    char ret[3];
    x <<= 3;
    ret[0] = 0xb0 | y;
    ret[1] = x & 0xf;
    ret[2] = 0x10 | (x>>4);
    HAL_I2C_Mem_Write(&hi2c1,0x78,0x00,1,ret,3,1);
}
void lcd_init(I2C_HandleTypeDef c){
    hi2c1 = c;
    HAL_I2C_Mem_Write(&hi2c1,0x78,0x00,1,init_,4,1);
    char dat[8] = {0};
    for(uint8_t r = 0;r< 8;r++){
        lcd_move(0,r);
        for(uint8_t c = 0;c<16;c++){
            HAL_I2C_Mem_Write(&hi2c1,0x78,0x40,1,dat,8,1);
        }
    }
    lcd_move(0,0);
}
void lcd_printc(char chr,char cursor){
    lcd_move(cursor&0xf,cursor>>4);
    // rot
    HAL_I2C_Mem_Write(&hi2c1,0x78,0x40,1,font8x8_basic[chr&0x7F],8,1);

}
//wip
void lcd_printmap(){
    char dat[8] = {0};
    for(uint8_t r = 0;r< 8;r++){
        move_lcd(0,r);
        for(uint8_t c = 0;c<16;c++){
            HAL_I2C_Mem_Write(&hi2c1,0x78,0x40,1,dat,8,1);
        }
    }
}