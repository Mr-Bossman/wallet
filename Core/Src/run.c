#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include "aes.h"
#include "sha256.h"
#include "encrypt.h"
#include "secp256k1.h"
#include "secp256k1_recovery.h"
#include "eeprom.h"
#include "Cstring_func.h"
#include "stdbool.h"
char *commands[] = {"help", "sign", "priv", "pub", "del", "sel", "signh", "signrech", "signrec"};

typedef struct encrypted_data
{
  uint8_t sum[32];
  uint8_t enc_priv[32];
  uint8_t pub[33];
} encrypted_data;

encrypted_data dat;
uint32_t save(uint32_t index, encrypted_data *in)
{
  return FLASH_WRITE_DATA(index, (uint8_t*)in, sizeof(encrypted_data));
}
uint32_t read(uint32_t index, encrypted_data *in)
{
  return FLASH_READ_DATA(index,  (uint8_t*)in, sizeof(encrypted_data));
}
uint16_t count()
{
  return FLASH_LEN_DATA();
}

uint16_t first_aval()
{
  encrypted_data data;
  size_t i = 0;
  for (; i < count(); i++)
  {
    read(i, &data);
    if (chk_null((const char*)&data, sizeof(encrypted_data)) || chk_chr((const char*)&data,0xff, sizeof(encrypted_data)))
      break;
  }
  return i;
}
//offset in sp
char *next_non_sp(char *p, size_t offset, size_t mlen)
{
  char *kill = strpnlen(p, mlen);
  char *l = 0;
  for (size_t i = 0; i < offset; i++)
  {
    l = p;
    while (*p++ != ' ')
      if (p >= kill)
        return 0;
    if (p == l)
      return 0;
    l = p;
    while (*p++ == ' ')
      if (p >= kill)
        return 0;
    if (p == l)
      return 0;
  }
  return p;
}

// bad code
int parse_params(char *command, char **params, int Nparams)
{
  int i = 0;
  char *p = next_non_sp(command, i++, 100);
  while (p != 0)
  {
    p = next_non_sp(command, i++, 100);
    if (p == strpnlen(command, 100))
      break;
    if (i - 2 < Nparams)
      params[i - 2] = p - 1;
  }

  if (i - 2 != Nparams)
    return -1;
  for (size_t b = 0; b < Nparams; b++)
  {
    char *ptr = memchr(params[b], ' ', 100);
    if(ptr)
      *ptr = 0;
  }
  return 0;
}

void help(char *command)
{
  size_t sz = sizeof(commands) / sizeof(char *);
  while (sz--)
    printf("%s ", commands[sz]);
  printf("\n%s\n", command);
}

int get_privkey(encrypted_data *in, uint8_t *pass, uint8_t priv[32])
{
  memcpy(priv, in->enc_priv, 32);
  return decrypt(priv, 32, (uint8_t *)pass, in->sum);
}

void save_privkey(uint8_t *seed, uint8_t *pass, encrypted_data *out)
{
  uint8_t priv[32];
  uint8_t sum[32];
  SHA256((uint8_t *)seed, priv);
  for (int i = 0; i < 32; i++)
    printf("%02x", priv[i]);
  printf("\n");
  secp256k1_context *ctx;
  secp256k1_pubkey pub;
  ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);
  secp256k1_ec_pubkey_create(ctx, &pub, priv);
  unsigned int sz = 33;
  secp256k1_ec_pubkey_serialize(ctx, out->pub, &sz, &pub, SECP256K1_EC_COMPRESSED);

  for (int i = 0; i < 33; i++)
    printf("%02x", out->pub[i]);
  printf("\n");
  encrypt_raw((uint8_t *)priv, 32, (uint8_t *)pass, sum);
  for (int i = 0; i < 32; i++)
    printf("%02x", sum[i]);
  printf("\n");
  memcpy(out->sum, sum, 32);
  memcpy(out->enc_priv, priv, 32);
  secp256k1_context_destroy(ctx);
}

void sign(uint8_t *message, uint8_t priv[32], uint8_t sig[64])
{
  uint8_t hash[32];
  SHA256((uint8_t *)message, hash);
  secp256k1_context *secp256k1_context_sign;
  secp256k1_context_sign = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);
  secp256k1_ecdsa_sign(secp256k1_context_sign, (secp256k1_ecdsa_signature*)sig, hash, priv, NULL, NULL);
  secp256k1_context_destroy(secp256k1_context_sign);
}
uint8_t hex_byte(uint8_t *str)
{
  char tmp[3] = {0};
  tmp[0] = str[0];
  tmp[1] = str[1];
  unsigned long int ret;
  ret = strtoul(tmp, NULL, 16);
  return (uint8_t)ret;
}
void signh(uint8_t *hex, uint8_t priv[32], uint8_t sig[64])
{
  uint8_t hash[32];
  //if (strlen(hex) != 64)
  //  return;
  for (int i = 0; i < 32; i++)
  {
    hash[i] = hex_byte(hex + (i * 2));
  }

  secp256k1_context *secp256k1_context_sign;
  secp256k1_context_sign = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);
  secp256k1_ecdsa_sign(secp256k1_context_sign, (secp256k1_ecdsa_signature*)sig, hash, priv, NULL, NULL);
  secp256k1_context_destroy(secp256k1_context_sign);
}
void signrech(uint8_t *hex, uint8_t priv[32], uint8_t sig[65])
{
  uint8_t hash[32];
  //if (strlen(hex) != 64)
  //  return;
  for (int i = 0; i < 32; i++)
  {
    hash[i] = hex_byte(hex + (i * 2));
  }
  for (int i = 0; i < 32; i++)
    printf("%02x", hash[i]);
  printf("\n");
  secp256k1_context *secp256k1_context_sign;
  secp256k1_context_sign = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);
  secp256k1_ecdsa_sign_recoverable(secp256k1_context_sign, (secp256k1_ecdsa_recoverable_signature*)sig, hash, priv, NULL, NULL);
  secp256k1_context_destroy(secp256k1_context_sign);
}
void signrec(uint8_t *message, uint8_t priv[32], uint8_t sig[65])
{
  uint8_t hash[32];
  SHA256((uint8_t *)message, hash);
  secp256k1_context *secp256k1_context_sign;
  secp256k1_context_sign = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);
  secp256k1_ecdsa_sign_recoverable(secp256k1_context_sign, (secp256k1_ecdsa_recoverable_signature*)sig, hash, priv, NULL, NULL);
  secp256k1_context_destroy(secp256k1_context_sign);
}

void signrech_parse(char *command)
{
  char *params[2] = {0};
  if (parse_params(command, params, 2))
  {
    printf("Syntax is wrong must be password then hex\n");
    return;
  }

  uint8_t priv[32];
  uint8_t sig[65];
  get_privkey(&dat, (uint8_t *)params[0], priv);
  signrech((uint8_t *)params[1], priv, sig);
  memset(priv, 0, 32);
  for (int i = 0; i < 65; i++)
    printf("%02x", sig[i]);
  printf("\n");
}
void signrec_parse(char *command)
{
  char *params[2] = {0};
  if (parse_params(command, params, 2))
  {
    printf("Syntax is wrong must be password then message\n");
    return;
  }

  uint8_t priv[32];
  uint8_t sig[64];
  get_privkey(&dat, (uint8_t *)params[0], priv);
  signrec((uint8_t *)params[1], priv, sig);
  memset(priv, 0, 32);
  for (int i = 0; i < 64; i++)
    printf("%02x", sig[i]);
  printf("\n");
}

void signh_parse(char *command)
{
  char *params[2] = {0};
  if (parse_params(command, params, 2))
  {
    printf("Syntax is wrong must be password then hex\n");
    return;
  }
  uint8_t priv[32];
  uint8_t sig[64];
  get_privkey(&dat, (uint8_t *)params[0], priv);
  signh((uint8_t *)params[1], priv, sig);
  memset(priv, 0, 32);
  for (int i = 0; i < 64; i++)
    printf("%02x", sig[i]);
  printf("\n");
}

void sign_parse(char *command)
{
  char *params[2] = {0};
  if (parse_params(command, params, 2))
  {
    printf("Syntax is wrong must be password then message\n");
    return;
  }

  uint8_t priv[32];
  uint8_t sig[64];
  get_privkey(&dat, (uint8_t *)params[0], priv);
  sign((uint8_t *)params[1], priv, sig);
  memset(priv, 0, 32);
  for (int i = 0; i < 64; i++)
    printf("%02x", sig[i]);
  printf("\n");
}
void priv_parse(char *command)
{
  char *params[2] = {0};
  if (parse_params(command, params, 2))
  {
    printf("Syntax is wrong must be password then seed\n");
    return;
  }

  save_privkey((uint8_t *)(params[1]), (uint8_t *)params[0], &dat);
  encrypted_data data;
  for (size_t i = 0; i < count(); i++)
  {
    read(i, &data);
    if (!memcmp(&dat, &data, sizeof(encrypted_data)))
      return;
  }
  save(first_aval(), &dat);
}

void pub_parse(char *command)
{
  char *p = next_non_sp(command, 1, strlen(command));
  if (p == strpnlen(command,100) && p)
  {
    printf("Syntax is wrong must be password then message\n");
    return;
  }
  
  encrypted_data data;
  size_t index = 0;
  for (size_t i = 0; i < count(); i++)
  {
    read(i, &data);
    if (chk_null((const char*)&data, sizeof(encrypted_data)) || chk_chr((const char*)&data,0xff, sizeof(encrypted_data)))
      continue;
    if (memcmp(&dat, &data, sizeof(encrypted_data)))
      printf("%u - ", index + 1);
    else
      printf("%u > ", index + 1);
    for (int b = 0; b < 33; b++)
      printf("%02x", data.pub[b]);
    printf("\n");
    index++;
  }
}

void del_parse(char *command)
{
  char *params[1] = {0};
  if (parse_params(command, params, 1))
  {
    printf("Syntax is wrong must be id\n");
    return;
  }
  encrypted_data data;
  int sel = atoi(params[0]);
  if (!sel)
  {
    printf("NaN\n");
    return;
  }
  sel -= 1;
  size_t index = 0;
  size_t i =0;
  while(1){
    if(i >= count()){
      printf("there is no n'th pubkey\n");
      return;
    }
    read(i++, &data);
    if (chk_null((const char*)&data, sizeof(encrypted_data)) || chk_chr((const char*)&data,0xff, sizeof(encrypted_data))) 
    continue;
    if (sel == index)break;
    index++;
  }
  i--;
  memset(&data, 0xff, sizeof(encrypted_data));
  save(i, &data);
  return;
}
void sel_parse(char *command)
{
  char *params[1] = {0};
  if (parse_params(command, params, 1))
  {
    printf("Syntax is wrong must be id\n");
    return;
  }
  encrypted_data data;
  int sel = atoi(params[0]);
  if (!sel)
  {
    printf("NaN\n");
    return;
  }
  sel -= 1;
  size_t index = 0;
  size_t i =0;
  while(1){
    if(i >= count()){
      printf("there is no n'th pubkey\n");
      return;
    }
    read(i++, &data);
    if (chk_null((const char*)&data, sizeof(encrypted_data)) || chk_chr((const char*)&data,0xff, sizeof(encrypted_data))) 
    continue;
    if (sel == index)break;
    index++;
  }
  memcpy(&dat, &data, sizeof(encrypted_data));
  for (int i = 0; i < 33; i++)
    printf("%02x", data.pub[i]);
  printf("\n");
  return;
}
int wallet_main(char *command)
{
  size_t offset = (size_t)memchr(command, ' ', strlen(command));
  if (offset == 0)
    offset = (size_t)command + strlen(command) - 1;
  size_t sz = sizeof(commands) / sizeof(char *);
  int i = 0;
  for (; i < sz; i++)
  {
    if (strncmp(command, commands[i], offset - (size_t)command) == 0)
    { // bad code
      break;
    }
  }
  if (i == sz)
  {
    help("not valid cmd");
    return -1;
  }
  switch (i)
  {
  case 0:
    help(command);
    break;
  case 1:
    sign_parse(command);
    break;
  case 2:
    priv_parse(command);
    break;
  case 3:
    pub_parse(command);
    break;
  case 4:
    del_parse(command);
    break;
  case 5:
    sel_parse(command);
    break;
  case 6:
    signh_parse(command);
    break;
  case 7:
    signrech_parse(command);
    break;
  case 8:
    signrec_parse(command);
    break;
  default:
    help("baaaad ");
  }
  return 0;
}