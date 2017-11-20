# include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

static GtkWidget *edit[2];    // Массив для полей ввода  

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

void click(GtkWidget *widget, GtkWidget *entry) {                 // Проверочная функция по передачи данных из полей ввода

  gchar *IP;
  gint PORT; 

  IP = (gchar*)gtk_entry_get_text(GTK_ENTRY(edit[0]));
  PORT = atoi((gchar*)gtk_entry_get_text(GTK_ENTRY(edit[1])));
  g_print ("IP:  %s\n", IP);
  g_print ("PORT:  %d\n", PORT);

}

int main( int argc, char *argv[] ) {
  /* Описываем виджеты GTK */

  GtkWidget *label;   // Метка
  GtkWidget *window;  // Главное окно (может содержать только один виджет!)
  GtkWidget *button;  // Кнопка
  GtkWidget *grid;    // Grid
  
  gtk_init(&argc, &argv);  // Инициализируем GTK+

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);             // Создаем главное окно
  gtk_window_set_title(GTK_WINDOW(window),"Сервер");        // Устанавливаем заголовок окна

  gint width_w = 300;                                                    // Ширина окна
  gint height_w = 150;                                                   // Высота окна

  gtk_window_set_default_size (GTK_WINDOW(window), width_w, height_w);   // Установка размеров окна (width * height) при запуске приложения 

  gint border = 20;                                                      // Размер рамки
  gtk_container_set_border_width (GTK_CONTAINER(window), border);        // Устанавливаем внутренние границы

  grid = gtk_grid_new();                                       // Создание grid
  gtk_container_add(GTK_CONTAINER(window), grid);              // Вставляем grid в главное окно

  label = gtk_label_new("Ip-адрес сервера");                   // Создаем label 
  gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);          // Помещаем label в grid, аргументы 3-6 задают позицию виджета, 
                                                               // 3 - номер колонки от левого края, 4 - номер строки от верхнего края, 
                                                               // 5 - число колонок, на которые распространяется виджет,
                                                               // 6 - число строк, на которые распространяется виджет  

  gint width_e = 200;                                                       // ширина поля ввода
  gint height_e = 0;                                                        // высота поля ввода

  edit[0] = gtk_entry_new();                                                // создание первого поля ввода  
  gtk_widget_set_size_request(edit[0], width_e, height_e);                  // устанавливаем размеры поля ввода
  gtk_grid_attach(GTK_GRID(grid), edit[0], 2, 0, 1, 1);
  
  gtk_entry_set_text(GTK_ENTRY(edit[0]), "Введите IP-адрес сервера");       // Иницилизация начальной строки в поле ввода(edit[0])
  
  GtkWidget *label1;                                                    // Метка

  label1 = gtk_label_new("Порт сервера");                               // Создаем label1 
  gtk_grid_attach(GTK_GRID(grid), label1, 0, 1, 1, 1);                  // Помещаем label1 в grid

  edit[1] = gtk_entry_new();                                               // Созданиe поля ввода
  gtk_entry_set_text(GTK_ENTRY(edit[1]), "Введите порт сервера");          // Иницилизация начальной строки в поле ввода(edit[1]) 
  gtk_grid_attach(GTK_GRID(grid), edit[1], 2, 1, 1, 1);

  GtkWidget *button1;                                                        // объявлеие кнопки(button)

  button1 = gtk_button_new_with_label("Запуск");                             // Создаем button
  gtk_grid_attach(GTK_GRID(grid), button1, 0, 2, 1, 1);                      // Помещаем button в grid 
  g_signal_connect(GTK_BUTTON(button1), "clicked", G_CALLBACK(click), NULL); // вызываем функцию click по нажатию button
  
  button1 = gtk_button_new_with_label("Выход");                                           // создаем button1
  g_signal_connect(GTK_BUTTON(button1), "clicked", G_CALLBACK(gtk_main_quit), NULL);      // вызываем функцию gtk_main_quit по нажатию
                                                                                          // button1 
  gtk_grid_attach(GTK_GRID(grid), button1, 2, 2, 1, 1);                                   // Помещаем button1 в grid             

  gtk_window_set_position (GTK_WINDOW(window), GTK_WIN_POS_CENTER); // Задает начальную позицию окна при запуске

  gtk_widget_show_all(window);  // Показываем окно вместе с виджетами

  gtk_widget_show_all(window);  // Показываем окно вместе с виджетами
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL); // выйти из программы по закрытию


  gtk_main();                                                 // Приложение переходит в цикл ожидания

  return 0;
}
