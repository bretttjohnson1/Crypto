#include <stdio.h>
#include <stdlib.h>

int hammingdist(char string1[], char string2[]){

   char *one = string1;
   char *two = string2;
   int count = 0;
   while((*one)!='\0' || (*two)!='\0'){
      char bytes = (*two)^(*one);

      for(int a=0;a<8;a++){
        //printf("%d",(bytes&1)==1 );
         if((bytes&1)==1){
            count++;

         }
         bytes>>=1;
      }
      //printf("\n");
      one++;
      two++;
   }
   return count;
}

int char_hammingdist(char one, char two){

   char bytes = one^two;
   int count = 0;
   for(int a=0;a<8;a++){
     //printf("%d",(bytes&1)==1 );
      if((bytes&1)==1){
         count++;
         }
      bytes>>=1;
   }
   return count;
}

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
   FILE *f = fopen("6.txt","rb");
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

   for(size_t a = 0;a<bufferlength;a++){
      printf("%c", *(filebuffer+a));
   }
   char *hexvals = base64tohex(filebuffer,bufferlength);
   long hexlength = bufferlength*3/4;


   int keyscores[40];
   for(int a = 0;a<40;a++)keyscores[a]=0;
   for(int a = 2;a<40;a++){
      int ham = 0;
      int count = 0;
      for(int c = 0;c<hexlength-2*a;c+=2*a){
         count++;
         for(int b = 0;b<a;b++){
            char one = *(hexvals+b+c);
            char two = *(hexvals+a+b+c);
            ham+=char_hammingdist(one,two);
         }
      }

      printf("%d %d\n", ham*1000/a/count, a);
      keyscores[a]+=(ham*1000/a/count);
   }
   int low = 0xFFFFFFF;
   int keysize = -1;
   for(int a =2;a<40;a++){
      if(low>keyscores[a]){
         low=keyscores[a];
         keysize=a;
      }
      printf("keysize: %d score:%d \n",a,keyscores[a]);
   }
   printf("keysize: %d score: %d\n",keysize,keyscores[keysize]);

   char likelykey[keysize];
   for(int a =0;a<keysize;a++){
      int bestscore = 0;
      char bestchar = 0;
      for(int character = 0;character<255;character++){
         int score = 0;
         for(int b = 0;b<hexlength-keysize;b+=keysize){
            char output = character^*(hexvals+a+b);

            if(((int)'A'<=output && output<=(int)'Z') || ((int)'a'<=output && output<=(int)'z') || output==32){
               score++;
            }
         }
         if(score>bestscore){
            bestscore=score;
            bestchar=character;
         }
      }
      printf("bestscore: %d %d ",bestscore,bestchar);
      likelykey[a]=bestchar;
   }

   printf("\n" );
   for(int a =0;a<keysize;a++)
      printf("%c",likelykey[a]);

   printf("\n\n");
   for(size_t a = 0;a<hexlength;a++){
      printf("%c",hexvals[a]^likelykey[a%keysize]);
   }

   printf("%02hhx",(char)(hexvals[hexlength-1]^likelykey[(hexlength-1)%keysize]));

   free(filebuffer);
   free(hexvals);
   return 0;


}
