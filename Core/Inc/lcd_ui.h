#ifndef LCD_UI_H
#define LCD_UI_H
#include "cmsis_os.h"
#include "main.h"
#include <stdio.h>
size_t password(char *str,size_t max);
void lcd_ui_main(void *arg);
#endif