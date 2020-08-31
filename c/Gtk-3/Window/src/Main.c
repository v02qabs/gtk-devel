#include <gtk/gtk.h>

int main(int argc, char** argv)
{
  //親ウィジェットwindowを指定
  GtkWidget *window;
  
  //GTK+の初期化とオプション解析
  gtk_init(&argc,&argv);
  
  //ウィンドウの作成
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  
  //ウィンドウのサイズ設定
  gtk_widget_set_size_request(window, 300, 200);
  
  //ウィンドウを閉じる時にプログラムも終了させる
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  
  //ウィンドウを表示
  gtk_widget_show(window);
  
  //GTK+のメイン関数（ループ）
  gtk_main();
  
  return 0;
}