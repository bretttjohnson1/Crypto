#include <stdio.h>

int main(){
   FILE *file = fopen("4.txt","r");
   char buff[62];
   while(fgets(buff, 62,file)){
      for(int character=0x00;character<0xFF;character++){
         char *index = buff;
         char output[sizeof(buff)/sizeof(char)/2];
         int score = 0;
         for(int a = 0;a<sizeof(output)/sizeof(char);a++){
            sscanf(index, "%2hhx", &output[a]);
            index+=2;
            output[a]=output[a]^character;
            if(( (int)'A'<=output[a] && output[a]<=(int)'Z') || ((int)'a'<=output[a] && output[a]<=(int)'z') || output[a]==32)
               score++;
         }
         if(score>(sizeof(output)/sizeof(char)-2)){
            for(int a = 0;a<sizeof(output)/sizeof(char);a++){
               printf("%c",output[a]);
            }
            printf("\n");
         }
      }
   }
}
