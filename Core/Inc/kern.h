#ifndef _RETARGET_H__
#define _RETARGET_H__

#include "stm32wbxx_hal.h"
#include <sys/stat.h>

void print_trace (void);
void RetargetInit(UART_HandleTypeDef *huart);
int _isatty(int fd);
int _write(int fd, char* ptr, int len);
int _close(int fd);
int _lseek(int fd, int ptr, int dir);
int _read(int fd, char* ptr, int len);
int _fstat(int fd, struct stat* st);

void* __real_realloc(void *, size_t);
void* __wrap_realloc(void *ptr, size_t sz);
void __real_free(void *);
void __wrap_free(void *ptr);


#endif //#ifndef _RETARGET_H__