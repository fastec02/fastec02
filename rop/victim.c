#include <stdio.h>
int main()
{
	char name[64];
	printf("%p\n",name);
	puts("What's your name");
	gets(name);
	printf("Hello, %s!\n",name);
	return 0;
}