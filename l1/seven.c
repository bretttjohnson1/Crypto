#include <stdio.h>
#include <stdlib.h>
#include "simpleAES.h"

int base64val(char symbol){
   if((int)symbol>='A' && (int)symbol<='Z'){
      return (int)symbol-65;
   }
   else if((int)symbol>='a' && (int)symbol<='z'){
      return (int)symbol-71;
   }
   else if((int)symbol>='0' && (int)symbol<='9'){
      return (int)symbol+4;
   }
   else if(symbol=='+'){
      return 62;
   }
   else if(symbol=='/'){
      return 63;
   }
   return -1;
}

char * base64tohex(char *base64string,int length){
   char *hexstring = malloc(sizeof(char)*length*3/4);
   int index = 0;
   int hexindex = 0;
   while(index<length){
      long twentyfourbitvalue = base64val(base64string[index])*262144+base64val(base64string[index+1])*4096
      +base64val(base64string[index+2])*64+base64val(base64string[index+3]);
      hexstring[hexindex+2] = twentyfourbitvalue%256;
      hexstring[hexindex+1] = (twentyfourbitvalue>>8)%256;
      hexstring[hexindex] = (twentyfourbitvalue>>16)%256;
      index+=4;
      hexindex+=3;
   }
   return hexstring;
}

int main(){
   FILE *f = fopen("7.txt","rb");
   char *filebuffer;
   long filelength = 0;
   long bufferlength = 0;
   fseek(f,0,SEEK_END);
   filelength = ftell(f);
   rewind(f);

   for(long a =0;a<filelength;a++){
      char c = getc(f);
      if(c!=10){
         bufferlength++;
      }
   }

   rewind(f);
   filebuffer = (char *)malloc((bufferlength+1)*sizeof(char));
   for(long a =0;a<bufferlength;a++){
      char c = getc(f);
      if(c!=10){
         *(filebuffer+a)=c;
      }else{
         a--;
      }
   }
   *(filebuffer+bufferlength)='\0';
   fclose(f);

   char *hexvals = base64tohex(filebuffer,bufferlength);
   long hexlength = bufferlength*3/4;

   unsigned char block[16];
   unsigned char key[] = "YELLOW SUBMARINE";
   for(long a  = 0;a<hexlength;a++){
      if(a%16==0 && a!=0){
         AES_decrypt(block,key);
         for(int b =0;b<16;b++)
            printf("%c", block[b]);
      }
      block[a%16] = hexvals[a];
   }
   AES_decrypt(block,key);
   for(int b =0;b<16;b++)
      printf("%c", block[b]);
   free(hexvals);
   free(filebuffer);
}
