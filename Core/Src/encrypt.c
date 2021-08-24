#include "encrypt.h"
//data must be a string padded with 0s to the neerst 16 bytes 
int encrypt(uint8_t*  data,uint8_t*  pass,uint8_t sum[32]){
  int len = strlen((char*) data);
  int diff = len&0xf;
  if(diff){
    int checks = 16-diff;
    len += checks;
    for(int i = 0;i < checks;i++){
      if(data[len+i])
        return -1;
    }
  }
  SHA256(pass,sum);
	unsigned char key1[176];
  unsigned char key2[176];
	KeyExpansion(sum, key1);
  KeyExpansion(sum+16, key2);
  SHA256_len(data,sum,len);
	for (int i = 0; i < len; i += 16) {
		AESEncrypt(data+i, key1, data+i);
	}
	for (int i = 0; i < len; i += 16) {
		AESEncrypt(data+i, key2, data+i);
	}
  return 0;
}
//data must be a string padded with 0s to the neerst 16 bytes 
int encrypt_raw(uint8_t*  data,size_t len,uint8_t*  pass,uint8_t sum[32]){
  SHA256(pass,sum);
	unsigned char key1[176];
  unsigned char key2[176];
	KeyExpansion(sum, key1);
  KeyExpansion(sum+16, key2);
  SHA256_len(data,sum,len);
	for (int i = 0; i < len; i += 16) {
		AESEncrypt(data+i, key1, data+i);
	}
	for (int i = 0; i < len; i += 16) {
		AESEncrypt(data+i, key2, data+i);
	}
  return 0;
}


//data must be a string padded with 0s to the neerst 16 bytes 
int decrypt(uint8_t* data,size_t len,uint8_t*  pass,uint8_t  sum[32]){
  uint8_t checksum[32];
  SHA256(pass,checksum);
	unsigned char key1[176];
  unsigned char key2[176];
	KeyExpansion(checksum, key1);
  KeyExpansion(checksum+16, key2);
	for (int i = 0; i < len; i += 16) {
		AESDecrypt(data+i, key2, data+i);
	}
	for (int i = 0; i < len; i += 16) {
		AESDecrypt(data+i, key1, data+i);
	}
  SHA256_len(data,checksum,len);
  if(memcmp(checksum,sum,32))
    return -2;
  return 0;
}