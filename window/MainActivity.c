#include <gtk/gtk.h>

int main(int argc, char *argv[])
{
	gtk_init(&argc,&argv);
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "window");
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}

