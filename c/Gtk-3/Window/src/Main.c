#include <gtk/gtk.h>

int main(int argc, char** argv)
{
  //�e�E�B�W�F�b�gwindow���w��
  GtkWidget *window;
  
  //GTK+�̏������ƃI�v�V�������
  gtk_init(&argc,&argv);
  
  //�E�B���h�E�̍쐬
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  
  //�E�B���h�E�̃T�C�Y�ݒ�
  gtk_widget_set_size_request(window, 300, 200);
  
  //�E�B���h�E����鎞�Ƀv���O�������I��������
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  
  //�E�B���h�E��\��
  gtk_widget_show(window);
  
  //GTK+�̃��C���֐��i���[�v�j
  gtk_main();
  
  return 0;
}