#include <stdio.h>
#include <stdlib.h>
#include "simpleAES.h"




int mod(int a,int b){
   int ret = a%b;
   if(ret<0){
      ret+=b;
   }
   return ret;
}
unsigned char GF_x_two(unsigned char c){
   unsigned char h = (unsigned char)((signed char)c>>7);
   return (c<<1) ^ 0x1B & h;
}
/*
Following 2 tables taken from https://en.wikipedia.org/wiki/Rijndael_S-box
*/
const unsigned char S_BOX[256] =
{
   0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
   0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
   0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
   0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
   0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
   0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
   0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
   0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
   0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
   0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
   0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
   0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
   0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
   0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
   0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
   0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};
 const unsigned char INVERSE_S_BOX[256] =
 {
    0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
    0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
    0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
    0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
    0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
    0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
    0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
    0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
    0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
    0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
    0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
    0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
    0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
    0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
    0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D
 };

 unsigned char key_rcon(unsigned char times){
    if(times==0)return 0x8d;
    unsigned char val = 1;
    while(times>1){
      val = GF_x_two(val);
      times--;
   }
   return val;
}
void key_rotate(unsigned char *four_byte_str){
   char buffer = four_byte_str[0];
   for(short a =3;a>=0;a--){
      char tmp = buffer;
      buffer = four_byte_str[a];
      four_byte_str[a] = tmp;
   }
}
/*
Learned from http://www.samiam.org/key-schedule.html
Code not copied, just Instructions followed (also names taken)
*/
void key_schedule_inner(unsigned char *four_byte_str, unsigned char i){
   key_rotate(four_byte_str);
   for(unsigned char a = 0;a<4;a++)
      four_byte_str[a] = S_BOX[four_byte_str[a]];
   four_byte_str[0] ^= key_rcon(i);
}

void subBytes(unsigned char *grid){
   for(unsigned char a =0;a<16;a++){
      grid[a]=S_BOX[grid[a]];
   }
}
void inverse_subBytes(unsigned char *grid){
   for(unsigned char a =0;a<16;a++){
      grid[a]=INVERSE_S_BOX[grid[a]];
   }
}
/*
Figured out how to do this with help from http://csrc.nist.gov/publications/fips/fips197/fips-197.pdf page 11-12
Code is mine
*/
void inverseMixTable(unsigned char *grid){
   for(unsigned char b = 0;b<4;b++){
      unsigned char ninecol[4];// 9*current column
      unsigned char elevencol[4];// 11*current column
      unsigned char thirteencol[4];// 13*current column
      unsigned char fourteencol[4];// 14*current column
      for(unsigned char a = 0;a<4;a++){
         ninecol[a] = GF_x_two(GF_x_two(GF_x_two(grid[a+b*4])))^grid[a+b*4];
         elevencol[a] =  GF_x_two(GF_x_two(GF_x_two(grid[a+b*4])))^GF_x_two(grid[a+b*4])^grid[a+b*4];
         thirteencol[a] =  GF_x_two(GF_x_two(GF_x_two(grid[a+b*4])))^GF_x_two(GF_x_two(grid[a+b*4]))^grid[a+b*4];
         fourteencol[a] =  GF_x_two(GF_x_two(GF_x_two(grid[a+b*4])))^GF_x_two(GF_x_two(grid[a+b*4]))^GF_x_two(grid[a+b*4]);
      }
      grid[b*4] = fourteencol[0]^elevencol[1]^thirteencol[2]^ninecol[3];
      grid[b*4+1] = fourteencol[1]^elevencol[2]^thirteencol[3]^ninecol[0];
      grid[b*4+2] = fourteencol[2]^elevencol[3]^thirteencol[0]^ninecol[1];
      grid[b*4+3] = fourteencol[3]^elevencol[0]^thirteencol[1]^ninecol[2];
   }
}

/*
Code for this section was based heavily off of https://en.wikipedia.org/wiki/Rijndael_mix_columns#MixColumns C implementation
Understanding gained (sort of) from http://csrc.nist.gov/publications/fips/fips197/fips-197.pdf page 11-12
*/
void mixTable(unsigned char * grid){
   for(unsigned char b = 0;b<4;b++){
      unsigned char col[4];
      unsigned char col_x_two_gl[4];

      for(unsigned char a = 0;a<4;a++){
         col[a]=grid[a+b*4];
         col_x_two_gl[a] = GF_x_two(col[a]);
      }
      grid[b*4] = col_x_two_gl[0]^col_x_two_gl[1]^col[1] ^col[2]^col[3];
      grid[b*4+1] = col[0]^col_x_two_gl[1]^col_x_two_gl[2]^col[2]^col[3];
      grid[b*4+2] = col[0]^col[1]^col_x_two_gl[2]^col_x_two_gl[3]^col[3];
      grid[b*4+3] = col[0]^col_x_two_gl[0]^col[1]^col[2]^col_x_two_gl[3];
   }
}

void shiftRows(unsigned char * grid){
   for(unsigned char a =0;a<4;a++){
      unsigned char row[4];
      for(unsigned char b = 0;b<4;b++){
         row[mod(b-a,4)] = grid[b*4+a];
      }
      for(unsigned char i = 0;i<4;i++){
         grid[a+i*4]=row[i];
      }
   }
}

void inverseshiftRows(unsigned char * grid){
   for(unsigned char a =0;a<4;a++){
      char row[4];
      for(unsigned char b = 0;b<4;b++){
         row[mod(b+a,4)] = grid[b*4+a];
      }
      for(unsigned char i = 0;i<4;i++){
         grid[a+i*4]=row[i];
      }

   }
}
/*
Code taken from http://www.samiam.org/key-schedule.html
*/
void expand_key(unsigned char * key){
   unsigned char i = 1;
   unsigned char c  = 16;
   unsigned char t[4];
   while(c<176){
      for(unsigned char a = 0;a<4;a++){
         t[a] = key[c-4+a];
      }
      if(c%16==0){
         key_schedule_inner(t,i);
         i++;
      }
      for(unsigned char a =0;a<4;a++){
         key[c] = key[c-16]^t[a];
         c++;
      }
   }
}

void add_round_key(unsigned char *bytes,unsigned char *key,unsigned char keyindex){
   for(unsigned char a = 0;a<16;a++){
      bytes[a]^=key[a+keyindex];
   }
}

void AES_encrypt(unsigned char *bytes,unsigned char *key){
   unsigned char *expanded_key = malloc(176*sizeof(char));
   for(unsigned char a = 0;a<16;a++)expanded_key[a]=key[a];
   expand_key(expanded_key);
   add_round_key(bytes,expanded_key,0);
   for(unsigned char a = 1;a<=10;a++){
      subBytes(bytes);
      shiftRows(bytes);
      if(a!=10)mixTable(bytes);
      add_round_key(bytes,expanded_key,a*16);
   }
}
void AES_decrypt(unsigned char *bytes,unsigned char *key){
   unsigned char *expanded_key = malloc(176*sizeof(char));
   for(int a = 0;a<16;a++)expanded_key[a]=key[a];
   expand_key(expanded_key);
   for(unsigned char a =10;a>0;a--){
      add_round_key(bytes,expanded_key,a*16);
      if(a!=10)inverseMixTable(bytes);
      inverseshiftRows(bytes);
      inverse_subBytes(bytes);
   }
   add_round_key(bytes,expanded_key,0);
   free(expanded_key);
}
