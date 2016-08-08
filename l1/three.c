#include <stdio.h>

int main(){
   const char encoded[] = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
   for(int character=0x00;character<0xFF;character++){
      const char *index = encoded;
      int score = 0;
      char out[sizeof(encoded)/sizeof(char)/2];
      for(size_t a=0;a<sizeof(out)/sizeof(char);a++){
         sscanf(index,"%2hhx",&out[a]);
         index+=2;
         out[a] = out[a]^character;
         if(( (int)'A'<=out[a] && out[a]<=(int)'Z') || ((int)'a'<=out[a] && out[a]<=(int)'z') || out[a]==32)
            score++;
      }
      //printf("%d %d \n ",score,(int)(sizeof(out)/sizeof(char)-10));
      if(score>(sizeof(out)/sizeof(char)-6)){
            for(size_t a=0;a<sizeof(out)/sizeof(char);a++){
               printf("%c",out[a]);
            }
            printf("\n");
      }
   }

}
