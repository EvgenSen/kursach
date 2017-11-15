# include <gtk/gtk.h>

/* выводит приветствие */
void welcome (GtkButton *button, gpointer data) {
        /* виджеты */
        GtkWidget *dialog;
        GtkWidget *label;
        GtkWidget *content_area;

        /* создать диалог */
        dialog = gtk_dialog_new_with_buttons("Заголовок окна",
                                             NULL,
                                             GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                             GTK_STOCK_OK,
                                             GTK_RESPONSE_ACCEPT,
                                             NULL);

        /* получить контейнер, в который будем пихать метку */
        content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

        /* сама метка */
        label = gtk_label_new("\n\nСообщение\n\n");
        gtk_container_add(GTK_CONTAINER(content_area), label);
        gtk_widget_show(label);

        /* запускаем диалог */
        gtk_dialog_run(GTK_DIALOG(dialog));
        /* а потом прячем */
        gtk_widget_destroy(dialog);
}

int main( int argc, char *argv[] ) {
  /* Описываем виджеты GTK */
  GtkWidget *label;   // Метка
  GtkWidget *window;  // Главное окно (может содержать только один виджет!)
  GtkWidget *button;  // Кнопка
  GtkWidget *table;   // Таблица 

  gtk_init(&argc, &argv);  // Инициализируем GTK+

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);                 // Создаем главное окно
  gtk_window_set_title(GTK_WINDOW(window),"Приложение");        // Устанавливаем заголовок окна
  int border = 20;
  gtk_container_set_border_width (GTK_CONTAINER(window), border);   // Устанавливаем внутренние границы

  gint rows = 2;
  gint columns = 2;

  table = gtk_table_new(rows, columns, TRUE);                    // Создание таблицы row * columsns

  gtk_container_add(GTK_CONTAINER(window), table);              // Вставляем таблицу в главное окно

  button = gtk_button_new_with_label("кнопка №1");              // Создаем button 

  gtk_table_attach_defaults(GTK_TABLE(table), button, 0, 1, 0, 1);    // Помещаем кнопку в таблицу 

  g_signal_connect(GTK_BUTTON(button), "clicked", G_CALLBACK(welcome), NULL); // вызываем функцию welcome по нажатию

  button = gtk_button_new_with_label("кнопка №2"); 

  gtk_table_attach_defaults(GTK_TABLE(table), button, 0, 1, 1, 2);     

  label = gtk_label_new("Запись №1");                                      // Соpдаем label 
  gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 0, 1);          // Помещаем label в таблицу 

  int width = 700;
  int height = 200;

  gtk_window_set_default_size (GTK_WINDOW(window), width, height);  // Установка размеров width * height окна при запуске приложения 

  gboolean resize = 1;

  gtk_window_set_resizable(GTK_WINDOW(window), resize);     // дает возможность пользователю растягивать окно, resize = 0 - нельзя, 
                                                            // resize = 1 - можно

  gtk_window_set_position (GTK_WINDOW(window), GTK_WIN_POS_CENTER); // Задает позицию приложения при запуске

  
  gtk_widget_show_all(window);  // Показываем окно вместе с виджетами

  g_signal_connect(GTK_BUTTON(button), "clicked", G_CALLBACK(welcome), NULL); // вызываем функцию welcome по нажатию

  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL); // выйти из программы по закрытию

  gtk_main();                                                 // Приложение переходит в цикл ожидания
  return 0;
}