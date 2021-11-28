#include <gtk/gtk.h>

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	g_print("Hello.");
	g_print("\n");

	GtkWidget *window;

