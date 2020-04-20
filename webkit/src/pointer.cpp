#include <iostream>

int main()
{
	printf("Hello\n");
	char *s0 = "Hello";
	char s1[sizeof(s0)];
	sprintf(s1, "%s", s0);
	printf("%s\n", s1);
	return 0;
}

