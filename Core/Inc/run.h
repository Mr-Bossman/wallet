#ifndef RUN_H
#define RUN_H
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include "aes.h"
#include "sha256.h"
#include "encrypt.h"
#include "secp256k1.h"
#include "secp256k1_recovery.h"
#include "Cstring_func.h"
#include "stdbool.h"
/*
uint32_t Random(){
    return 0;
}

// get a signmature to see if code is valid
uint64_t get_program_signature(){
    return 0;
}

//get user sig so they can put custom code on
uint64_t get_user_signature(){
    return 0;
}*/
size_t pub_list(char ***strs);
void save_privkey_int(uint8_t *seed, uint8_t *pass);
void del_num(int sel);
int wallet_main(char *s);
typedef struct
{
    uint8_t sum[32];
    uint8_t enc_priv[32];
    uint8_t pub[33];
} encrypted_data;

#endif
