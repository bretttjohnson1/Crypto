
#include <stdio.h>
int hamming(unsigned char *array, unsigned char *array1, int length){
	int hamming = 0;
	for(int i = 0; i < length * 8; i++){
      printf("%d %d |", i/8,i);
		hamming+=(((array[i/8] >> i%8))^((array1[i/8] >> i%8)))&1;
		//printf("%d %d", bit1, bit2);
	}

	return hamming;
}

int main(){
	unsigned char array[] = "this is a test";
	unsigned char array1[] = "wokka wokka!!!";
	int distance = hamming(array, array1, sizeof(array)-1);
	printf("%d", distance);
	return 0;
}
