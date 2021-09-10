#include "lcd_ui.h"
#include "ssd1306.h"
#include "pics.h"
#include "cmsis_os.h"
#include <stdbool.h>
#include <string.h>

size_t password(char *str,size_t max);
void lcd_ui_main(void *arg){
    I2C_HandleTypeDef* i2c = (I2C_HandleTypeDef*)arg;
    lcd_init(*i2c);
    lcd_clear();
    for(size_t i = 0; i < sizeof(map)/(16*64);i++){
        lcd_printmap(map[i]);
        osDelay(10);
    }
    while (1) {
        osDelay(10);
    }
}

void menu(void *arg){

}
// fix readpin static val
int read_pin(uint32_t grp,uint32_t io);
size_t password(char *str,size_t max){

    memset(str,0,max);
    size_t pos = 0;
    while(1){
        if(read_pin(TSC_GROUP1_IDX,TSC_GROUP1_IO4) < 2000)
            pos++;
        if(read_pin(TSC_GROUP3_IDX,TSC_GROUP3_IO2) < 2000)
            pos--;
        /* clamp pos to 0-max */
        if(max == pos)
            pos = max-1;
        if(pos > max)
            pos = 0;
        /* end clamping*/
        if(read_pin(TSC_GROUP1_IDX,TSC_GROUP1_IO2) < 2000)
            str[pos]++;
        if(read_pin(TSC_GROUP1_IDX,TSC_GROUP1_IO3) < 2000)
            str[pos]--;

        while(1){
            bool ex = false;
            ex |= read_pin(TSC_GROUP1_IDX,TSC_GROUP1_IO3) < 2000;
            ex |= read_pin(TSC_GROUP1_IDX,TSC_GROUP1_IO2) < 2000;
            ex |= read_pin(TSC_GROUP1_IDX,TSC_GROUP1_IO4) < 2000;
            ex |= read_pin(TSC_GROUP3_IDX,TSC_GROUP3_IO2) < 2000;
            if(!ex) break;
            osDelay(50);
        }
        /* allow wraping through visable ascii chars*/
        if(!str[pos])
            str[pos] = ' ';
        if(str[pos] < ' ')
            str[pos] = '~';
        if(str[pos] > '~')
            str[pos] = ' ';
        /* end wraping code */
        lcd_printstr(str,pos);
        if(read_pin(TSC_GROUP3_IDX,TSC_GROUP3_IO4) < 2000)
            break;
        osDelay(10);
        // add sleep
    }
    return pos;
}