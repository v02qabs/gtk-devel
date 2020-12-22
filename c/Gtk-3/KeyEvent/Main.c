#include <gtk/gtk.h>
#include <stdlib.h>

static gboolean signal_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
	gint i;
	if(event->state &GDK_CONTROL_MASK && event->keyval == 'd')
	{
		g_print("Hello.\n");
	}

	return TRUE;
	
}



int main(int argc, char *argv[])
{
	GtkWidget *window;
	gtk_init(&argc, &argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(window), "key-press-event", G_CALLBACK(signal_key_press), NULL);
	gtk_widget_show(window);
	gtk_main();
	return 0;
}

