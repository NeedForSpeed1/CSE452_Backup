//program1
#include <stdio.h>

int main(){
	
	//define 2 variables listed in instructions
	float a = 1;
	int b = 2;
	char c = 'a';

	//print their addresses 
	printf("a: %p, b: %p, c: %p\n", &a, &b, &c);

	return 0;
}

