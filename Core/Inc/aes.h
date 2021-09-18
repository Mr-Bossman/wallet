#pragma once
#ifndef AES_H
#define AES_H
#include <stdio.h>
#include <string.h>
#include "structures.h"
void AESDecrypt(unsigned char *encryptedMessage, unsigned char *expandedKey, unsigned char *decryptedMessage);
void AESEncrypt(unsigned char *message, unsigned char *expandedKey, unsigned char *encryptedMessage);
#endif