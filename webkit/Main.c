#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	GtkWidget *window, *webview;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	webview = webkit_web_view_new();
	gtk_container_add(GTK_CONTAINER(window), webview);
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}

	
