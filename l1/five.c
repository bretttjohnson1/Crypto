#include <stdio.h>

int main(){
   const char msg[] = "Burning 'em, if you ain't quick and nimble I go crazy when I hear a cymbal";
   //const char msg[] = "qwertyuiopasdfghjklzxcvbnmmjybgrvdwqscrgnjuklojgfderuhgtbvchsxdazzdtcgybhunjimkolp";
   char key[] = "ICE";
   char out[sizeof(msg)/sizeof(char)-1];
   for(int a = 0;a< sizeof(out)/sizeof(char);a++){
      out[a] = msg[a]^key[a%(sizeof(key)/sizeof(char)-1)];
   }
   for(int a = 0;a< sizeof(out)/sizeof(msg[0]);a++)
      printf("%02hhx",out[a]);

   printf("\n");
   for(int a = 0;a< sizeof(out)/sizeof(msg[0]);a++)
      printf("%c",out[a]^key[a%(sizeof(key)/sizeof(char)-1)]);


}
