#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	//変数
	char pwdf[1000];
	char pwd[1000];
	char *fargv0 = argv[1];
	char appname[sizeof(fargv0)];

	//dir.
	getcwd(pwd, sizeof(pwd));
	//appname
	sprintf(appname , "%s", fargv0);
	//output
	sprintf(pwdf, "file://%s/%s", pwd);
	printf("fullpath: %s\n", pwdf);
	return 0;
}

