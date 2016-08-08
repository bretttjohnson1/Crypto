#include <stdio.h>

int main(){
   char codes[] = "899b25bc4b86b8e37d40e4f9dfbf5036490798e177d11e1f251d1ae3b19fcbfe";
   char hexcodes[(sizeof(codes)-1)/2];
   for(int a = 0;a<sizeof(codes)-1;a+=2){
      sscanf(&codes[a],"%2hhx",&hexcodes[a/2]);

   }
   for(int a = 0;a<sizeof(hexcodes);a++){
      printf("%2hhx", hexcodes[a]);
   }

}
