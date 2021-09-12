#include "lcd_ui.h"
#include "ssd1306.h"
#include "pics.h"
#include "cmsis_os.h"
#include <stdbool.h>
#include <string.h>
#include "run.h"

// fix readpin static val
int read_pin(uint32_t grp,uint32_t io);
size_t lists(char **str,size_t count,bool nums);
size_t password(char *str,size_t max,size_t line);

void wait(){
 while(read_pin(TSC_GROUP3_IDX,TSC_GROUP3_IO4) > 2000)
    osDelay(10);
while(read_pin(TSC_GROUP3_IDX,TSC_GROUP3_IO4) < 2000)
    osDelay(50);
}
void list(){
    lcd_clear();
    char **lens;
    size_t len = pub_list(&lens);
    lists(lens,len,true);
    for(size_t i = 0; i < len;i++)
        free(lens[i]);
    free(lens);
}
void del(){
    lcd_clear();
    char **lens;
    size_t len = pub_list(&lens);
    del_num(lists(lens,len,true));
    for(size_t i = 0; i < len;i++)
        free(lens[i]);
    free(lens);
}
void priv_(bool det){
    char pass[50] = {0};
    char priv[50] = {0};
    lcd_clear();
    lcd_printstr("password",255,0);
    password(pass,sizeof(pass),2<<4);
    if(det){
        lcd_clear();
        lcd_printstr("seed",255,0);
        password(priv,sizeof(priv),2<<4);
    } else {
        //rand
    }
    save_privkey_int(priv,pass);
}
void lcd_ui_main(void *arg){
    I2C_HandleTypeDef* i2c = (I2C_HandleTypeDef*)arg;
    lcd_init(*i2c);
    lcd_clear();
    for(size_t i = 0; i < sizeof(map)/(16*64);i++){
        lcd_printmap(map[i]);
        osDelay(10);
    }
    char *commands[] = {"priv", "pub", "del","privr"};
    lcd_clear();
    while (1) {
        size_t i = menu(commands,sizeof(commands)/sizeof(char *));
        switch (i){
            case 0: priv_(false);
            break;
            case 1: list();
            break;
            case 2: del();
            break;
            case 3: priv_(true);
            break;
            default:
            break;
        }
        osDelay(10);
    }
}

size_t menu(char **str,size_t size){
    size_t sel = 0;
    lcd_clear();
    while(1){
        if(read_pin(TSC_GROUP1_IDX,TSC_GROUP1_IO2) < 2000)
            sel++;
        if(read_pin(TSC_GROUP1_IDX,TSC_GROUP1_IO3) < 2000)
            sel--;
        while(1){
            bool ex = false;
            ex |= read_pin(TSC_GROUP1_IDX,TSC_GROUP1_IO3) < 2000;
            ex |= read_pin(TSC_GROUP1_IDX,TSC_GROUP1_IO2) < 2000;
            if(!ex) break;
            osDelay(50);
        }
        /* clamp pos to 0-size */
        if(size == sel)
            sel = 0;
        if(sel > size)
            sel = size-1;
        /* end clamping*/
        for(size_t b = 0;b < size;b++){
            lcd_printlc(' ',0,b);
            for(size_t i = 0;str[b][i];i++){
                lcd_printlc(str[b][i],i+1,b);
            }
        }
        lcd_printlc('>',0,sel);
        if(read_pin(TSC_GROUP3_IDX,TSC_GROUP3_IO4) < 2000)
            break;
        osDelay(10);
    }
    while(read_pin(TSC_GROUP3_IDX,TSC_GROUP3_IO4) < 2000)
            osDelay(50);
    return sel;
}
size_t lists(char **str,size_t count,bool nums){
    size_t line = 0;
    size_t window = 0;
    const size_t lineCount = 5;
    while(1){
        for(size_t i = window;i < window+lineCount;i++){
            if(i < count){
                size_t offset = 0;
                if(nums){
                    char nums[4] = {0};
                    snprintf(nums,3,(i == line)?"%u>":"%u ",i);
                    offset = strlen(nums);
                    lcd_printstr(nums,255,(i-window)<<4);
                }
                for (size_t b = 0; b < 10; b++){
                    if(!str[i][b])
                        break;
                    lcd_printlc(str[i][b],b+offset,i-window);
                }
            } else{
                for (size_t b = 0; b < 12; b++)
                    lcd_printlc(0,b,i-window);
            }
        }
        if(read_pin(TSC_GROUP1_IDX,TSC_GROUP1_IO4) < 2000)
            line++;
        if(read_pin(TSC_GROUP3_IDX,TSC_GROUP3_IO2) < 2000)
            line--;
        if(read_pin(TSC_GROUP1_IDX,TSC_GROUP1_IO2) < 2000)
            line++;
        if(read_pin(TSC_GROUP1_IDX,TSC_GROUP1_IO3) < 2000)
            line--;
        /* clamp pos to 0-max */
        if(count == line)
            line = 0;
        if(line > count)
            line = count-1;
        /* end clamping*/
        if(line > window+(lineCount-1))
            window = line-(lineCount-1);
        if(line < window)
            window = line;
        while(1){
            bool ex = false;
            ex |= read_pin(TSC_GROUP1_IDX,TSC_GROUP1_IO3) < 2000;
            ex |= read_pin(TSC_GROUP1_IDX,TSC_GROUP1_IO2) < 2000;
            ex |= read_pin(TSC_GROUP1_IDX,TSC_GROUP1_IO4) < 2000;
            ex |= read_pin(TSC_GROUP3_IDX,TSC_GROUP3_IO2) < 2000;
            if(!ex) break;
            osDelay(50);
        }
        if(read_pin(TSC_GROUP3_IDX,TSC_GROUP3_IO4) < 2000)
            break;
        osDelay(5);
        // add sleep
    }
    while(read_pin(TSC_GROUP3_IDX,TSC_GROUP3_IO4) < 2000)
        osDelay(50);
    return line;
}


size_t password(char *str,size_t max,size_t line){
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
        lcd_printstr(str,pos,line);
        if(read_pin(TSC_GROUP3_IDX,TSC_GROUP3_IO4) < 2000)
            break;
        osDelay(10);
        // add sleep
    }
    while(read_pin(TSC_GROUP3_IDX,TSC_GROUP3_IO4) < 2000)
        osDelay(50);
    return pos;
}