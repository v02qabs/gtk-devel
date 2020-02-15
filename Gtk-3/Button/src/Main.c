#include <gtk/gtk.h>

int main(int argc, char *argv[])
{
	gtk_init(&argc,&argv);
	g_print("Hello.");
	GtkWidget *window, *button;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Hello GUI GTK+-3.0");
	gtk_widget_set_size_request(window, 300, 200);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	button = gtk_button_new();
	gtk_widget_show(button);
	gtk_container_add(GTK_CONTAINER(window),button);
	gtk_widget_show(window);
	gtk_main();
	return 0;
}

