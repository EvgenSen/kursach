#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lib-trace.h"

#define FILE_NAME "input.txt"

static GtkWidget *edit[2];    // Массив для полей ввода
static GtkWidget *combo;      // ComboBox для списка

int get_time ()
{
  struct timeval tv;
  gettimeofday(&tv, 0); 
  return (int)tv.tv_usec / 1000;
}

int read_array_from_file(int ** mass)
{
  FILE *f = NULL;
  char *line = NULL;
  size_t len = 0;
  int i = 0;

  f = fopen(FILE_NAME, "r");
  if (f == NULL) {
    trace_msg(ERR_MSG, "[%s] Can not open file\n",__FUNCTION__);
    return -1;
  }

  while (getline(&line, &len, f) != -1)
  {
    (*mass)=realloc((*mass), sizeof(int*)*(i+1));
    (*mass)[i] = atoi(line);
    i++;
  }
  trace_msg(DBG_MSG, "[%s] Read %d numbers from '%s' \n",__FUNCTION__,i, FILE_NAME);

  if (line != NULL) {
    free(line);
    line = NULL;
  }
  fclose(f);
  return i;
}

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

int check_ip(char *addr)
{
  if (!addr){
    trace_msg(ERR_MSG, "[%s] Empty value ", __FUNCTION__);
    return -1;
  }

  int x[4];
  int dot = 0;
  int i = 0;

  for (; i< (int)strlen((const char *)addr); i++)
  {
    if (addr[i] == '.')
      dot++;
    else if (addr[i] < 48 || addr[i] > 57) {
      trace_msg(ERR_MSG, "[%s] '%s' - Incorrect IP addres 1", __FUNCTION__,addr);
      return -1;
    }
  }

  if (dot != 3) {
    trace_msg(ERR_MSG, "[%s] '%s' - Incorrect IP addres 2", __FUNCTION__,addr);
    return -1;
  }

  if ((sscanf(addr, "%d.%d.%d.%d", &x[0], &x[1], &x[2], &x[3])) == 4)
  {
    for (i = 0; i < 4; i++)
      if (x[i] < 0 || x[i] > 255){
        trace_msg(ERR_MSG, "[%s] '%s' - Incorrect IP addres, out of range of values", __FUNCTION__,addr);
        return -1;
      }
    return 0;
  }
  trace_msg(ERR_MSG, "[%s] '%s' - Incorrect IP addres 3", __FUNCTION__,addr);
  return -1;
}

int check_port(gint port)
{
  if (port > 65536 || port <= 0)
  {
    trace_msg(ERR_MSG, "[%s] Incorrect port value '%d' ", __FUNCTION__,port);
    return 1;
  }
  return 0;
}

/* Обрабатываем входные данные и запускаем работу */
void click(GtkWidget *widget, GtkWidget *entry) {
  gchar *IP;
  gint PORT;

  IP = (gchar*)gtk_entry_get_text(GTK_ENTRY(edit[0]));
  PORT = atoi((gchar*)gtk_entry_get_text(GTK_ENTRY(edit[1])));

  if (check_ip(IP) || check_port(PORT))
    return;

  int start_t, stop_t;
  int * mass = NULL;
  int value;
  start_t = get_time();
  int k = read_array_from_file(&mass);

  trace_msg(DBG_MSG, "[%s] IP address:  %s\n",__FUNCTION__, IP);
  trace_msg(DBG_MSG, "[%s] Port:        %d\n",__FUNCTION__, PORT);
  switch(gtk_combo_box_get_active(GTK_COMBO_BOX(combo)))
  {  
    case 0:  
      trace_msg(DBG_MSG, "[%s] Action:      Find Max value in array\n",__FUNCTION__);
      break;
    case 1:  
      trace_msg(DBG_MSG, "[%s] Action:      Find Min value in array\n",__FUNCTION__);
      break;
    case 2:  
      trace_msg(DBG_MSG, "[%s] Action:      Sort array\n",__FUNCTION__);
      break;
    default:  
      trace_msg(ERR_MSG, "[%s] Unknown action \n",__FUNCTION__);
      break;
  }
  stop_t = get_time();
  trace_msg(DBG_MSG, "[%s] Time: %d ms\n",__FUNCTION__,stop_t - start_t);
}

int main( int argc, char *argv[] ) {
  /* Описываем виджеты GTK */
  GtkWidget *label_ip;      // Метка (Текст перед полем)
  GtkWidget *label_port;
  GtkWidget *label_action;
  GtkWidget *window;        // Главное окно (может содержать только один виджет!)
  GtkWidget *button_start;  // Инициализация кнопки
  GtkWidget *button_exit;
  GtkWidget *grid;          // Grid

  gtk_init(&argc, &argv);  // Инициализируем GTK+

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);             // Создаем главное окно
  gtk_window_set_title(GTK_WINDOW(window),"Сервер");        // Устанавливаем заголовок окна

  gint width_w = 350;    // Ширина окна
  gint height_w = 350;   // Высота окна

  gtk_window_set_default_size (GTK_WINDOW(window), width_w, height_w);   // Установка размеров окна (width * height) при запуске приложения

  gint border = 20;                                                      // Размер рамки
  gtk_container_set_border_width (GTK_CONTAINER(window), border);        // Устанавливаем внутренние границы

  grid = gtk_grid_new();                                       // Создание grid
  gtk_container_add(GTK_CONTAINER(window), grid);              // Вставляем grid в главное окно

  label_ip = gtk_label_new("IP-адрес сервера");                // Создаем label
  gtk_grid_attach(GTK_GRID(grid), label_ip, 0, 0, 1, 1);       // Помещаем label в grid, аргументы 3-6 задают позицию виджета, 
                                                               // 3 - номер колонки от левого края, 4 - номер строки от верхнего края,
                                                               // 5 - число колонок, на которые распространяется виджет,
                                                               // 6 - число строк, на которые распространяется виджет

  gint width_e = 250;                                                       // ширина поля ввода
  gint height_e = 0;                                                        // высота поля ввода

  edit[0] = gtk_entry_new();                                                // создание первого поля ввода  
  gtk_widget_set_size_request(edit[0], width_e, height_e);                  // устанавливаем размеры поля ввода
  gtk_entry_set_text(GTK_ENTRY(edit[0]), "Введите IP-адрес сервера");       // Иницилизация начальной строки в поле ввода(edit[0])
  gtk_grid_attach(GTK_GRID(grid), edit[0], 2, 0, 1, 1);

  label_port = gtk_label_new("Порт сервера");                               // Создаем label
  gtk_grid_attach(GTK_GRID(grid), label_port, 0, 1, 1, 1);                  // Помещаем label в grid

  edit[1] = gtk_entry_new();                                               // Созданиe поля ввода
  gtk_widget_set_size_request(edit[1], width_e, height_e);                 // устанавливаем размеры поля ввода
  gtk_entry_set_text(GTK_ENTRY(edit[1]), "Введите порт сервера");          // Иницилизация начальной строки в поле ввода(edit[1])
  gtk_grid_attach(GTK_GRID(grid), edit[1], 2, 1, 1, 1);

  button_start = gtk_button_new_with_label("Запуск");                             // Создаем button
  gtk_grid_attach(GTK_GRID(grid), button_start, 0, 3, 1, 1);                      // Помещаем button в grid 
  g_signal_connect(GTK_BUTTON(button_start), "clicked", G_CALLBACK(click), NULL); // вызываем функцию click по нажатию button

  button_exit = gtk_button_new_with_label("Выход");                                           // создаем button1
  g_signal_connect(GTK_BUTTON(button_exit), "clicked", G_CALLBACK(gtk_main_quit), NULL);      // вызываем функцию gtk_main_quit по нажатию
  gtk_grid_attach(GTK_GRID(grid), button_exit, 2, 3, 1, 1);                                   // Помещаем button1 в grid

  label_action = gtk_label_new("Действие");                         // Создаем label
  gtk_grid_attach(GTK_GRID(grid), label_action, 0, 2, 1, 1);        // Помещаем label в grid

  combo = gtk_combo_box_text_new(); // Создаем ComboBox
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo), NULL, "Find Max");
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo), NULL, "Find Min");
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo), NULL, "Sort array");
  gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);  // номер списка по умолчанию
  gtk_grid_attach(GTK_GRID(grid), combo, 2, 2, 1, 1); // добавляем ComboBox в окно

  gtk_window_set_position (GTK_WINDOW(window), GTK_WIN_POS_CENTER); // Задает начальную позицию окна при запуске

  gtk_widget_show_all(window);  // Показываем окно вместе с виджетами
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL); // выйти из программы по закрытию

  gtk_main();                                                 // Приложение переходит в цикл ожидания

  return 0;
}
