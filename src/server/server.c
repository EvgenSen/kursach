# include <gtk/gtk.h>

/* выводит приветствие */
void welcome (GtkButton *button, gpointer data)
{
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
        label = gtk_label_new("\n\nИ тебе привет, %username!%");
        gtk_container_add(GTK_CONTAINER(content_area), label);
        gtk_widget_show(label);

        /* запускаем диалог */
        gtk_dialog_run(GTK_DIALOG(dialog));
        /* а потом прячем */
        gtk_widget_destroy(dialog);
}

int main( int argc, char *argv[] ) {
  /* Описываем виджеты GTK */
  GtkWidget *label;  // Метка
  GtkWidget *window; // Главное окно (может содержать только один виджет!)
  GtkWidget *button; // Кнопка

  gtk_init(&argc, &argv);  // Инициализируем GTK+

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);                 // Создаем главное окно
  gtk_window_set_title(GTK_WINDOW(window),"Заголовок окна!");   // Устанавливаем заголовок окна
  gtk_container_set_border_width (GTK_CONTAINER(window), 50);   // Устанавливаем внутренние границы

  //label = gtk_label_new( "Здравствуй, мир!" );     // Создаем метку с текстом
  //gtk_container_add(GTK_CONTAINER(window), label); // Вставляем метку в главное окно

  button = gtk_button_new_with_label("Привет, кнопка!"); 
  gtk_container_add(GTK_CONTAINER(window), button); 

  gtk_widget_show_all(window);  // Показываем окно вместе с виджетами

  g_signal_connect(GTK_BUTTON(button), "clicked", G_CALLBACK(welcome), NULL); // вызываем функцию welcome по нажатию

  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL); // выйти из программы по закрытию
  gtk_main();                                                // Приложение переходит в цикл ожидания
  return 0;
}