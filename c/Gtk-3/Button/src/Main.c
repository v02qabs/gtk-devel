#include <gtk/gtk.h>


void show_message(char *message);

static void click_button(GtkWidget widget, gpointer data)
{
	show_message("OK.");
	gtk_main_quit();
}

void show_message(char *message)
{
	g_print("%s\n", message);
}


int main(int argc, char *argv[])
{
	gtk_init(&argc,&argv);
	g_print("Hello.");
	GtkWidget *window, *button, *box;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Hello GUI GTK+-3.0");
	gtk_widget_set_size_request(window, 300, 200);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	button = gtk_button_new();
	button = gtk_button_new_with_label("OK");
	g_signal_connect(button, "clicked", G_CALLBACK(click_button), NULL);
	box = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	gtk_container_add(GTK_CONTAINER(box),button);
	gtk_container_add(GTK_CONTAINER(window), box);
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}

