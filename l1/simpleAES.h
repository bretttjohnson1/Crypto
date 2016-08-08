#ifndef simpleAES_header_file
#define simpleAES_header_file

int mod(int a,int b);

unsigned char GF_x_two(unsigned char c);

void key_rotate(unsigned char *four_byte_str);

void key_schedule_inner(unsigned char *four_byte_str, unsigned char i);

void subBytes(unsigned char *grid);

void inverse_subBytes(unsigned char *grid);

void inverseMixTable(unsigned char *grid);

void mixTable(unsigned char * grid);

void shiftRows(unsigned char * grid);

void inverseshiftRows(unsigned char * grid);

void expand_key(unsigned char * key);

void add_round_key(unsigned char *bytes,unsigned char *key,unsigned char keyindex);

void AES_encrypt(unsigned char *bytes,unsigned char *key);

void AES_decrypt(unsigned char *bytes,unsigned char *key);

#endif