#include "Cstring_func.h"
uint8_t chk_null(const char* ptr,size_t len){
  for(size_t i = 0;i < len;i++)
    if(*ptr++)
      return 0;
  return 1;
}
uint8_t chk_chr(const char* ptr,char chr,size_t len){
  for(size_t i = 0;i < len;i++)
    if(*ptr++ != chr)
      return 0;
  return 1;
}
char* strplen(char* ptr){
    while (*ptr++);
    return ptr;
}

char* strpnlen(char* ptr,size_t len) {
    size_t i =0;
    while ( i < len && *ptr++ ) ++i;
    return ptr;
}

