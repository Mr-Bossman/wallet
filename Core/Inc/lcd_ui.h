#ifndef LCD_UI_H
#define LCD_UI_H
#include "cmsis_os.h"
#include "main.h"
#include <stdio.h>
size_t password(char *str,size_t max,size_t line);
size_t menu(char **str,size_t size);
void lcd_ui_main(void *arg);
extern osThreadId lcd_task;
#endif