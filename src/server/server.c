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
  GtkWidget *grid;   // Контейнер

  gtk_init(&argc, &argv);  // Инициализируем GTK+

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);                   // Создаем главное окно
  gtk_window_set_title(GTK_WINDOW(window),"Заголовок окна!");     // Устанавливаем заголовок окна
  gtk_window_set_default_size ( GTK_WINDOW ( window ), 400, 600); // Установка начального размера окна
  gtk_container_set_border_width (GTK_CONTAINER(window), 50);     // Устанавливаем внутренние границы

  grid = gtk_grid_new (); // Создаем новый контейнер
  gtk_container_add (GTK_CONTAINER (window), grid); // Помещем контейнер в окно

  label = gtk_label_new( "Нажми на кнопку!" );      // Создаем метку с текстом
  /* Помещем метку в контейнер, параметры из 4 цифр:
   * позиция слева, позиция сверху, ширина и высота */
  gtk_grid_attach (GTK_GRID (grid), label, 0, 0, 2, 2);

  button = gtk_button_new_with_label("Первая кнопка");
  g_signal_connect(GTK_BUTTON(button), "clicked", G_CALLBACK(welcome), NULL); // вызываем функцию welcome по нажатию
  gtk_grid_attach (GTK_GRID (grid), button, 0, 2, 1, 1); // Помещем кнопку в контейнер

  button = gtk_button_new_with_label("Вторая кнопка");
  g_signal_connect(GTK_BUTTON(button), "clicked", G_CALLBACK(welcome), NULL); // вызываем функцию welcome по нажатию
  gtk_grid_attach (GTK_GRID (grid), button, 2, 2, 1, 1); // Помещем кнопку в контейнер

  gtk_widget_show_all(window);  // Показываем окно вместе с виджетами
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL); // выйти из программы по закрытию
  gtk_main();  // Приложение переходит в цикл ожидания
  return 0;
}
