#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include <stdio.h>
#include <unistd.h>

GtkWidget *init_gtk(char *filename)
{
	GtkWidget *window;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	WebKitWebView *webkit = WEBKIT_WEB_VIEW(webkit_web_view_new());
	webkit_web_view_load_uri(webkit, filename);
	gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(webkit));
	 g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	 //return window;
	  gtk_widget_set_size_request(window, 500, 500);
    return window;
}

int main(int argc, char *argv[])
{
    char *filename = argv[1];
		char fullpath[sizeof(filename)];
		sprintf(fullpath, "%s;", filename);
		//getcwd
		char pwd[1024];
		getcwd(pwd, sizeof(pwd));
		printf("cwd : %s\n", pwd);

		//add
		char fpath[1000];
		sprintf(fpath, "file://%s/%s",pwd, argv[1]);
		printf("%s\n", fpath);

		gtk_init(&argc, &argv);
		GtkWidget *window = init_gtk(fpath);
    gtk_widget_show_all(window);

	gtk_main();
	return 0;
}

	
