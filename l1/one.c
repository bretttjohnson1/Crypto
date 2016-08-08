#include <stdio.h>

static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};

int main(){
   const char input[] =  "5ECF+ivaM9hqo0C0wWwFreRAhfor2jPYaqNAtMFsBa3kQIX6K9oz2GqjQLTBbAWtlIoGxUWUEtE7P13yMm17Ug==";
   const char *hexstring = input;

   char bytes[sizeof(input)/sizeof(char)];
   for(size_t a=0;a<sizeof(bytes)/sizeof(char);a++){
      sscanf(hexstring, "%1hhx", &bytes[a]);
      hexstring+=1;
   }

   for(size_t a = 0;a<sizeof(bytes)/sizeof(char)-3;a+=3){
      int out = bytes[a]*256+bytes[a+1]*16+bytes[a+2];
      int big = out/64;
      int small = out%64;
      printf("%c%c",encoding_table[big],encoding_table[small]);
   }

}
