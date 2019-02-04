#include <gtk/gtk.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	gtk_init(&argc,&argv);
	g_printf("Hello.");
	exit(0);
	return 0;
}

