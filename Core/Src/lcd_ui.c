#include "lcd_ui.h"
#include "ssd1306.h"
#include "pics.h"
void lcd_ui_main(void *arg){
    I2C_HandleTypeDef* i2c = (I2C_HandleTypeDef*)arg;
    lcd_init(*i2c);
    lcd_clear();
    for(size_t i = 0; i < sizeof(map)/(16*64);i++){
        lcd_printmap(map[i]);
        osThreadYield();
        osDelay(1);
    }
    while (1) {
        osDelay(10);
    }
}