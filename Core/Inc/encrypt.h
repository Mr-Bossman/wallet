

#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <stdio.h>
#include <string.h>
#include "aes.h"
#include "sha256.h"
int encrypt_raw(uint8_t*  data,size_t len,uint8_t*  pass,uint8_t sum[32]);
int encrypt(uint8_t*  data,uint8_t* pass,uint8_t sum[32]);
int decrypt(uint8_t*  data,size_t len,uint8_t*  pass,uint8_t sum[32]);
#endif