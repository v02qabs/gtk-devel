#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	char pwd[1000];
	getcwd(pwd, sizeof(pwd)-1);
	printf("%s\n", pwd);
	return 0;
}

