#ifndef STRING_U_H
#define STRING_U_H
#include <stddef.h>
#include <stdint.h>
#include <string.h>
uint8_t chk_null(const char* ptr,size_t len);
char* strplen(char* ptr);
char* strpnlen(char* ptr,size_t len);
uint8_t chk_chr(const char* ptr,char chr,size_t len);
size_t strnlen(char* ptr,size_t len);
#endif