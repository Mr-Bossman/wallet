#ifndef STRING_U_H
#define STRING_U_H
#include <stddef.h>
#include <stdint.h>
#include <string.h>
uint8_t chk_null(const char* ptr,size_t len);
char* strplen(char* ptr);
char* strpnlen(char* ptr,size_t len);
#endif