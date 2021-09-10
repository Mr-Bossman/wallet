#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "cmsis_os.h"
#include "main.h"

#include "font8x8.h"
#include "ssd1306.h"

//assume const
char init_[] = {0xAE,0x8D,0x14,0xAF};

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
    //assume const
    HAL_I2C_Mem_Write(&hi2c1,0x78,0x00,1,init_,4,1);
}
void lcd_clear(){
    char dat[8] = {0};
    for(uint8_t r = 0;r< 8;r++){
        lcd_move(0,r);
        for(uint8_t c = 0;c<16;c++){
            HAL_I2C_Mem_Write(&hi2c1,0x78,0x40,1,dat,8,1);
        }
    }
    lcd_move(0,0);
}

void lcd_printc(const char chr,const char cursor){
    lcd_move(cursor&0xf,cursor>>4);
    char tmp[8] = {0};
    for(size_t b = 0;b< 8;b++)
        for(size_t bt = 0;bt< 8;bt++){
            char bit = (font8x8_basic[chr&0x7f][b] >> bt)&1;
            tmp[bt] |= bit << b;
        }
    HAL_I2C_Mem_Write(&hi2c1,0x78,0x40,1,tmp,8,1);

}


void lcd_printstr(const char *chr,char cursor){
    for(size_t index = 0; chr[index];index++){
        lcd_move(index&0xf,index>>4);
        char character[8];
        memcpy(character,font8x8_basic[chr[index]&0x7f],8);
        if(cursor == index){
            for(size_t i = 0;i < 8; i++)
                character[i] |= font8x8_basic['_'][i];
        }
        char tmp[8] = {0};
        for(size_t b = 0;b< 8;b++)
            for(size_t bt = 0;bt< 8;bt++){
                char bit = ( character[b]>> bt)&1;
                tmp[bt] |= bit << b;
            }
        HAL_I2C_Mem_Write(&hi2c1,0x78,0x40,1,tmp,8,1);
    }
}

void lcd_printmap(const char map[64][16]){
    for(uint8_t r = 0;r< 8;r++){
        lcd_move(0,r);
        for(uint8_t c = 0;c<16;c++){
            char dat[8] = {0};
            for(size_t b = 0;b< 8;b++)
                for(size_t bt = 0;bt< 8;bt++){
                    char bit = (map[b+(r*8)][c] >> bt)&1;
                    dat[bt] |= bit << b;
            }
            HAL_I2C_Mem_Write(&hi2c1,0x78,0x40,1,dat,8,1);
        }
    }
}