#include <stdio.h>

int main(){
   char string1[] = "1c0111001f010100061a024b53535009181c";
   char string2[] = "686974207468652062756c6c277320657965";
   char out1[sizeof(string1)/sizeof(char)];
   char out2[sizeof(string1)/sizeof(char)];
   char output[sizeof(string1)/sizeof(char)];
   for(size_t a=0;a<sizeof(string1)/sizeof(char)-1;a++){
      sscanf(&string1[a],"%1hhx",&out1[a]);
      sscanf(&string2[a],"%1hhx",&out2[a]);
      output[a] = out1[a]^out2[a];
      printf("%x",output[a]);
   }
}
