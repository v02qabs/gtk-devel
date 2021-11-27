#include <gtk/gtk.h>



void button_clicked(GtkWidget *widget, gpointer data)
{
	const gchar *text;
	text = gtk_entry_get_text(GTK_ENTRY(data));
	g_print("clicked : %s\n", text);
}



int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	GtkWidget *window;
	GtkWidget *vbox;
	GtkWidget *button;
	GtkWidget *save;
	GtkWidget *text_area;

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window), 400,400);
	vbox =	gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
	gtk_container_add(GTK_CONTAINER(window),vbox);
	text_area = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(vbox),text_area, TRUE,TRUE,0);
	button = gtk_button_new_with_label("SHOW");
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE,FALSE, 0);
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(button_clicked), &text_area);
	save = gtk_button_new_with_label("quit");
	gtk_box_pack_start(GTK_BOX(vbox),save, TRUE,TRUE, 0); 
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}

