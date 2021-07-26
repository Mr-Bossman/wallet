#include <stdio.h>
#include <string.h>
#include "aes.h"
#include "sha256.h"
#include "encrypt.h"
#include "secp256k1.h"
#include "secp256k1_recovery.h"



typedef struct encrypted_data {
    uint8_t sum[32];
    uint8_t enc_priv[32];
    uint8_t pub[33];
} encrypted_data;



encrypted_data dat;



void get_privkey(encrypted_data* in,uint8_t* pass,uint8_t priv[32]){
  memcpy(priv,in->enc_priv,32);
  decrypt(priv,32,(uint8_t*)pass,in->sum);
}

void save_privkey(uint8_t* seed,uint8_t* pass,encrypted_data* out){
    uint8_t priv[32];
    uint8_t sum[32];
    SHA256((uint8_t*)seed,priv);
    for(int i = 0; i < 32;i++)printf("%02x",priv[i]);
    printf("\n");
    secp256k1_context* ctx;
    secp256k1_pubkey pub;
    ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);
    secp256k1_ec_pubkey_create(ctx,&pub,priv);
    unsigned int sz = 33;
    secp256k1_ec_pubkey_serialize(ctx,out->pub,&sz,&pub,SECP256K1_EC_COMPRESSED);

    for(int i = 0; i < 33;i++)printf("%02x",out->pub[i]);
    printf("\n");
    
    encrypt_raw((uint8_t*)priv,32,(uint8_t*)pass,sum);
    memcpy(out->sum,sum,32);
    memcpy(out->enc_priv,priv,32);
    secp256k1_context_destroy(ctx);
}

void sign(uint8_t* message,uint8_t priv[32],uint8_t sigg[72]){
  uint8_t hash[32];
  SHA256((uint8_t*)message,hash);
  unsigned int sz = 72;
  secp256k1_ecdsa_signature sig;
  secp256k1_context* secp256k1_context_sign;
  secp256k1_context_sign = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);
  secp256k1_ecdsa_sign(secp256k1_context_sign, &sig, hash, priv, secp256k1_nonce_function_rfc6979, NULL);
  secp256k1_ecdsa_signature_serialize_der(secp256k1_context_sign,sigg,&sz,&sig);
  }

/*
    uint8_t p[32];
    osDelay(1000);

    SHA256((uint8_t*)"bruh",p);
    for(int i = 0; i < 32;i++)printf("%02x",p[i]);
    printf("\n");
    secp256k1_ecdsa_signature sig;
    uint8_t t[72];
    secp256k1_context* secp256k1_context_sign;
    secp256k1_context_sign = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);
    int ret = secp256k1_ecdsa_sign(secp256k1_context_sign, &sig, p, p, secp256k1_nonce_function_rfc6979, NULL);
    int sz = 72;
    secp256k1_ecdsa_signature_serialize_der(secp256k1_context_sign,t,&sz,&sig);
    for(int i = 0; i < 72;i++)printf("%02x%s",t[i],(!((i+1)%8))?"\n":"");
    */
void signh(uint8_t hash[32],uint8_t priv[32],uint8_t sig[64]){
  //sign
}
void help(char* command){
	printf("help exit sign save_priv get_pub save load q\n%s\n",command);
}

void sign_parse(char* command){
  int Nparams = 2;
  char * params[2];
  int i = 0;
  char* p = memchr(command,' ',strlen(command));
  while(p !=  0){
    while(*p == ' ')p++;
    if(p ==  strlen(command)+command)break;
    char* tmp = memchr(p+1,' ',strlen(command));
    if(!tmp)tmp = strlen(command)+command;
    if(i < Nparams){
      size_t sz = (tmp-p);
      params[i] = malloc(sz+1);
      params[i][sz] = 0;
      strncpy(params[i],p,sz);
      i++;
    } else {
        i++;
        break;
    }
    p = tmp;
  }
  if(i != Nparams){
  printf("Syntax is wrong must be password then message\n");
  if(i>=1)  free(params[0]);
  if(i>=2)  free(params[1]);
  return;
  }
  uint8_t priv[32];
  uint8_t sig[72];
  get_privkey(&dat,(uint8_t*)params[0],priv);
  sign((uint8_t*)params[1],priv,sig);
  memset(priv,0,32);
  for(int i = 0; i < 72;i++)printf("%02x",sig[i]);
  printf("\n");
  free(params[1]);
  free(params[0]);
}
void priv_parse(char*  command){
  int Nparams = 2;
  char * params[2];
  int i = 0;
  char* p = memchr(command,' ',strlen(command));
  while(p !=  0){
    while(*p == ' ')p++;
    if(p ==  strlen(command)+command)break;
    char* tmp = memchr(p+1,' ',strlen(command));
    if(!tmp)tmp = strlen(command)+command;
    if(i < Nparams){
      size_t sz = (tmp-p);
      params[i] = malloc(sz+1);
      params[i][sz] = 0;
      strncpy(params[i],p,sz);
      i++;
    } else {
        i++;
        break;
    }
    p = tmp;
  }
  if(i != Nparams){
  printf("Syntax is wrong must be password then message\n");
  if(i>=1)  free(params[0]);
  if(i>=2)  free(params[1]);
  return;
  }
  save_privkey((uint8_t*)(params[1]),(uint8_t*)params[0],&dat);
  free(params[1]);
  free(params[0]);
}


void pub_parse(char*  command){
  char* p = memchr(command,' ',strlen(command));
  while(*p == ' ')p++;
  if(p != command && p){
    printf("Syntax is wrong must be password then message\n");
    return;
  }
  for(int i = 0; i < 33;i++)printf("%02x",dat.pub[i]);
  printf("\n");
}




char* commands[] = {"help","exit","sign","save_priv","get_pub","save","load","q","signh"};

int wallet_main(char*command) {
    size_t offset = (size_t)memchr(command,' ',strlen(command));
    if(offset == 0)
      offset = (size_t)command + strlen(command)-1;
    size_t sz = sizeof(commands)/sizeof(char*);
    int i = 0;
    for(;i < sz;i++){
    if(strncmp(command,commands[i],offset-(size_t)command) == 0){ // bad code
      break;
    }
    }
    if(i == sz){
      help("2");
      return -1;
    }
    switch (i){
      case 0:
        help(command);
        break;
      case 1:
        break;
      case 2:
        sign_parse(command);
        break;
      case 3:
        priv_parse(command);
        break;
      case 4:
        pub_parse(command);
        break;
      case 5:
        break;
      case 6:
        break;
      case 7:
      break;
      case 8:
        break;
      default:
        help("3");
    }
  return 0;
}