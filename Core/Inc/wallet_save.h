#ifndef WS_H
#define WS_H
extern encrypted_data enc_sv;
int sdinit();
int sdclose();
int add_enc();
size_t size_enc();
int get_enc(size_t index);
int del_enc(size_t index);
#endif