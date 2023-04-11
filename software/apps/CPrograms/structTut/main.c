#include <stdio.h>

typedef struct Person
{
	//
	int age;
	float weight;
}person1;

int main()
{	

	// use for defining objects of the structure
	person1 p1;

	p1.age = 1;
	p1.weight = 10.2;

	person1 *ptr = (person1 *) 0x4000000;

	*ptr = p1; // dereference operator

    
	// use for printing the addresses
	printf("Person: %p", ptr);
	printf("\nAge: %p, Weight: %p\n", &p1.age, &p1.weight);

    return 0;
}
