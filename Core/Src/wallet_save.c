#include "stdio.h"
#include "eeprom.h"
#include "run.h"
#include "wallet_save.h"
static FILE *fptr = NULL;
encrypted_data enc_sv;
char* pass = "tmp";
int sdinit()
{
    if (!fptr)
        fptr = fopen("/wallet.dat", "w");
    return -(!fptr);
}

int sdclose()
{
    return fclose(fptr);
}

typedef struct {
    uint8_t sum[32];
    uint8_t eenc[112];
}packed;
//need to add fseek
int add_enc()
{
    packed filedat;
    memset(&filedat,0,sizeof(packed));
    memcpy(filedat.eenc,&enc_sv,sizeof(encrypted_data));
    encrypt_raw(filedat.eenc, 112, pass, filedat.sum);//pass?
    fseek(fptr, 0, SEEK_END);
    fwrite(&filedat,sizeof(packed),1, fptr);
    return 0;
}
size_t size_enc(){
    fseek(fptr, 0, SEEK_END);
    return ftell(fptr)/sizeof(packed);
}
int get_enc(size_t index)
{
    packed filedat;
    fseek(fptr, 0, sizeof(packed)*index);
    fwrite(&filedat,sizeof(packed),1, fptr);
    decrypt(filedat.eenc, 112, pass, filedat.sum);//pass?
    memcpy(&enc_sv,filedat.eenc,sizeof(encrypted_data));
    return 0;
}

int del_enc(size_t index)
{
        return 0;
}


/*

static encrypted_data dat;
uint32_t save(uint32_t index, encrypted_data *in)
{
    return FLASH_WRITE_DATA(index, (uint8_t *)in, sizeof(encrypted_data));
}
uint32_t read(uint32_t index, encrypted_data *in)
{
    return FLASH_READ_DATA(index, (uint8_t *)in, sizeof(encrypted_data));
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
        if (chk_null((const char *)&data, sizeof(encrypted_data)) || chk_chr((const char *)&data, 0xff, sizeof(encrypted_data)))
            break;
    }
    return i;
}

static int expand(size_t bytes, size_t index)
{
    uint16_t count = FLASH_LEN_DATA();
    if (count > index + 1)
    {
        // we need to shift blobs
        uint32_t offsets = FLASH_LEN_BLOBS(index);
        uint32_t total = FLASH_LEN_BLOBS(count - 1);
        uint32_t copyfact = total - offsets;

        {
        uint32_t blocks = copyfact / 8; // 8 byte blocks;
        uint8_t rem = copyfact % 8;
        uint8_t backBuffer[16] = {0};
        for (size_t i = 0; i < blocks; i++)
        {
            FLASH_READ(offsets + (i * 8), backBuffer + (8 * (i & 1)), 8);
            FLASH_WRITE(offsets + (i * 8), backBuffer + (8 * (~i & 1)), 8);
        }
        if(rem){
            FLASH_READ(offsets + (blocks * 8), backBuffer + (8 * (i & 1)), 8);
            FLASH_WRITE(offsets + (blocks * 8), backBuffer + (8 * ((i + 1) & 1)), 8);
        }0
        if(blocks)
            FLASH_WRITE(offsets + (blocks * 8), backBuffer + (8 * (~blocks & 1)), 8);
    }
    else
    {
        //we are the end of blob
    }
}


void Fmemmove(){
    uint8_t* from = (uint8_t*) src;
	uint8_t* to = (uint8_t*) dest;

	if (from == to || n == 0)
		return;
	if (to > from && to-from < (int)n) {

		int i;
		for(i=n-1; i>=0; i--)
			to[i] = from[i];
		return;
	}
	if (from > to && from-to < (int)n) {

		size_t i;
		for(i=0; i<n; i++)
			to[i] = from[i];
        return;
	}
	memcpy(dest, src, n);
}

*/