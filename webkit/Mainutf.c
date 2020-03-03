#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	GtkWidget *window;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	WebKitWebView *webkit = WEBKIT_WEB_VIEW(webkit_web_view_new());
	webkit_web_view_load_uri(webkit, "http://takos.chobi.net/");
	gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(webkit));
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}

	
