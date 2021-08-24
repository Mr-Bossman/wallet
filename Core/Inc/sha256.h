
#ifndef SHA256_H
#define SHA256_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
void SHA256Init(SHA256_CTX *ctx);
void SHA256Update(SHA256_CTX *ctx, const uint8_t data[], uint32_t len);
void SHA256Final(SHA256_CTX *ctx, uint8_t hash[]);
char* SHA256_hex(char* data);
void SHA256(uint8_t* data,	uint8_t hash[32]);
void SHA256_len(uint8_t* data,uint8_t hash[32],size_t strLen);
#endif