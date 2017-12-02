#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <time.h>
#include <sys/time.h>  
  
#include "lib-trace.h"
#include "lib-func.h"

#define FILE_NAME "input.txt"

static GtkWidget *edit[3];    // Массив для полей ввода
static GtkWidget *combo[3];      // ComboBox для списка
// static GtkWidget *combo_cl_2;      // ComboBox для списка
// static GtkWidget *combo_cl_3;      // ComboBox для списка

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

int check_port(int port)
{
  if (port > 65536 || port <= 0)
  {
    trace_msg(ERR_MSG, "[%s] Incorrect port value '%d' ", __FUNCTION__,port);
    return 1;
  }
  return 0;
}

int check_sameport(int port[], int size) {

  for(int i = 0; i < size - 1; i++) {
    if(port[i] == port[i+1]) {
      trace_msg(ERR_MSG, "[%s] Same ports '%d' and  '%d', choise other number of port", __FUNCTION__,port[i],port[i+1]);
      return 1;
    }
  }
  return 0;
}

int create_socket(int sock, int port_serv) {

  if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
    trace_msg(ERR_MSG, "[%s], Server: socket() failed", __FUNCTION__);
    exit(1);
  }

  struct sockaddr_in echoServAddr;

  /* Construct local address structure */
  memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
  echoServAddr.sin_family = AF_INET;                /* Internet address family */
  echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
  echoServAddr.sin_port = htons(port_serv);              /* Local port */

  /* Bind to the local address */
  if (bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0) {
    trace_msg(ERR_MSG, "[%s], Server: bind() failed", __FUNCTION__);
    exit(1);
  }

  if (listen(sock, 5) < 0) {
    trace_msg(ERR_MSG, "[%s], Server: listen() failed", __FUNCTION__);
    exit(1);
  }

  struct sockaddr_in echoClntAddr; /* Client address */
  unsigned int clntLen;            /* Length of client address data structure */

  /* Set the size of the in-out parameter */
  clntLen = sizeof(echoClntAddr);

  /* Mark the socket so it will listen for incoming connections */
  if ((sock = accept(sock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0) {
    trace_msg(ERR_MSG, "[%s], Server: accept() failed", __FUNCTION__);
    exit(1);
  }

  return sock;
}

void send_data(int sock, int* data, int size_of_data) {

  if (send(sock, data, size_of_data, 0) != size_of_data) {
    trace_msg(ERR_MSG, "[%s], Server: send() failed", __FUNCTION__);
    exit(1);
  }
  else {
    trace_msg(DBG_MSG, "[%s], Server: massive has sended.\n",__FUNCTION__);
  }
}

void send_value(int sock, int value) {

  if (send(sock, &value, sizeof(int), 0) != sizeof(int)) {
    trace_msg(ERR_MSG, "[%s], Server: send() failed", __FUNCTION__);
    exit(1);
  }
  else {
    trace_msg(DBG_MSG, "[%s], Server: value has sended.\n",__FUNCTION__);
  }
}

int recv_value(int sock, int value_cl) {

  int bytesRecv;

  if ((bytesRecv = recv(sock, &value_cl, sizeof(int), 0)) <= 0) {
    trace_msg(ERR_MSG, "[%s], Server: recv_value()) failed", __FUNCTION__);
    exit(1);
  } else {
    trace_msg(DBG_MSG, "[%s], Server: value has sended.\n",__FUNCTION__);
  }

  return value_cl;
}

int* recv_mass(int sock, int mass[], int size_of_mas) {

  int bytesRecv;

  if ((bytesRecv = recv(sock, mass, size_of_mas, 0)) <= 0) {
    trace_msg(ERR_MSG, "[%s], Server: recv_value()) failed", __FUNCTION__);
    exit(1);
  } else {
    trace_msg(DBG_MSG, "[%s], Server: value has sended.\n",__FUNCTION__);
  }

  return mass;
}
/* Обрабатываем входные данные и запускаем работу */
void click(GtkWidget *widget, GtkWidget *entry) {

  int n = 3;  
  int PORT[n];

  for(int i = 0; i < n; i++) {
    PORT[i] = atoi((gchar*)gtk_entry_get_text(GTK_ENTRY(edit[i])));
  }

  for(int i = 0; i < n; i++) {
    if (check_port(PORT[i]))
        return 0;
  }

  if (check_sameport(PORT, n))
    return 0;

  int start_t, stop_t;
  int * mass = NULL;
  int value_s;
  int value_cl;
  start_t = get_time();
  int k = read_array_from_file(&mass);

  /*Start Share mass */

  int **submas;                                             
  submas = malloc(sizeof(int*)*n);

  for(int j = 0; j < n; j++) {
    int i = 0;
    submas[j] = malloc(sizeof(int)*(k/n));
    for(int l = 0; l < k/n; l++) {
      submas[j][l] = mass[i];
      i++;
    }
  }

  /*End Share mass */
  trace_msg(DBG_MSG, "[%s] Port:        %d\n",__FUNCTION__, PORT[0]);

  int sock;
  int size_of_mas = sizeof(int)*(k/n);

  sock = create_socket(sock, PORT[0]);
  send_value(sock, k/n);
  send_data(sock, submas[0], size_of_mas);


  switch(gtk_combo_box_get_active(GTK_COMBO_BOX(combo[0])))
  {
    case 0:
      send_value(sock, FIND_MAX);
      value_s = find_value(submas[0], k/n , FIND_MAX);
      trace_msg(DBG_MSG, "[%s], Server: action - find Max value in array (%d) \n",__FUNCTION__, value_s);
      value_cl = recv_value(sock, value_cl);
      trace_msg(DBG_MSG, "[%s], Client: action - find Max value in array (%d) \n",__FUNCTION__, value_cl);
      break;
    case 1:
      send_value(sock, FIND_MIN);
      value_s = find_value(submas[0], k/n, FIND_MIN);
      trace_msg(DBG_MSG, "[%s], Server: action - find Min value in array (%d)\n",__FUNCTION__, value_s);
      value_cl = recv_value(sock, value_cl);
      trace_msg(DBG_MSG, "[%s], Client: action - find Min value in array (%d) \n",__FUNCTION__, value_cl);
      break;
    case 2:
      send_value(sock, SORT);
      submas[0] = sort(submas[0], k/n, SORT);
      trace_msg(DBG_MSG, "[%s], Server: action - Sort array\n",__FUNCTION__);
      for(int i = 0; i < k/n; i++) {
        trace_msg(DBG_MSG, "%d ", submas[0][i]);
      }
      send_value(sock, SORT);
      submas[0] = recv_mass(sock, submas[0], size_of_mas);
      trace_msg(DBG_MSG, "[%s], Client: action - Sort array\n",__FUNCTION__);
      for(int i = 0; i < k/n; i++) {
        trace_msg(DBG_MSG, "%d ", submas[0][i]);
      }
      break;
    default:
      trace_msg(ERR_MSG, "[%s], Server: action - Unknown action \n",__FUNCTION__);
      break;
  }

  stop_t = get_time();
  trace_msg(DBG_MSG, "[%s] Time: %d - %d = %d \n",__FUNCTION__, stop_t, start_t, (stop_t - start_t));
  trace_msg(DBG_MSG, "[%s] Time: %d ms\n",__FUNCTION__, (stop_t - start_t) < 0 ? 1000 - start_t + stop_t : stop_t - start_t);


}

int main( int argc, char *argv[] ) {
  /* Описываем виджеты GTK */
  GtkWidget *label_port;
  GtkWidget *label_action[3];
  // GtkWidget *label_action_cl_2;
  // GtkWidget *label_action_cl_3;
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

  gint width_e = 250;                                                       // ширина поля ввода
  gint height_e = 0;                                                        // высота поля ввода

  edit[0] = gtk_entry_new();                                                // создание первого поля ввода
  gtk_widget_set_size_request(edit[0], width_e, height_e);                  // устанавливаем размеры поля ввода
  label_port = gtk_label_new("Порт для клиента №1");                        // Создаем label
  gtk_grid_attach(GTK_GRID(grid), label_port, 0, 0, 1, 1);                  // Помещаем label в grid

  gtk_entry_set_text(GTK_ENTRY(edit[0]), "Введите порт для клиента №1");     // Иницилизация начальной строки в поле ввода(edit[0])
  gtk_grid_attach(GTK_GRID(grid), edit[0], 1, 0, 1, 1);

  label_action[0] = gtk_label_new("Действие");                                 // Создаем label
  gtk_grid_attach(GTK_GRID(grid), label_action[0], 0, 1, 1, 1);                // Помещаем label в grid

  combo[0] = gtk_combo_box_text_new(); // Создаем ComboBox
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo[0]), NULL, "Find Max");
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo[0]), NULL, "Find Min");
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo[0]), NULL, "Sort array");

  gtk_combo_box_set_active(GTK_COMBO_BOX(combo[0]), 0);                        // номер списка по умолчанию
  gtk_grid_attach(GTK_GRID(grid), combo[0], 1, 1, 1, 1);                       // добавляем ComboBox в окно

  edit[1] = gtk_entry_new();                                                // создание первого поля ввода
  gtk_widget_set_size_request(edit[1], width_e, height_e);                  // устанавливаем размеры поля ввода
  label_port = gtk_label_new("Порт для клиента №2");                        // Создаем label
  gtk_grid_attach(GTK_GRID(grid), label_port, 0, 2, 1, 1);                  // Помещаем label в grid

  gtk_entry_set_text(GTK_ENTRY(edit[1]), "Введите порт для клиента №2");    // Иницилизация начальной строки в поле ввода(edit[1])
  gtk_grid_attach(GTK_GRID(grid), edit[1], 1, 2, 1, 1);

  label_action[1] = gtk_label_new("Действие");                                 // Создаем label
  gtk_grid_attach(GTK_GRID(grid), label_action[1], 0, 3, 1, 1);                // Помещаем label в grid

  combo[1] = gtk_combo_box_text_new(); // Создаем ComboBox
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo[1]), NULL, "Find Max");
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo[1]), NULL, "Find Min");
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo[1]), NULL, "Sort array");
  gtk_combo_box_set_active(GTK_COMBO_BOX(combo[1]), 0); 
  gtk_grid_attach(GTK_GRID(grid), combo[1], 1, 3, 1, 1);                  // добавляем ComboBox в окно

  edit[2] = gtk_entry_new();                                                // создание первого поля ввода
  gtk_widget_set_size_request(edit[2], width_e, height_e);                  // устанавливаем размеры поля ввода
  label_port = gtk_label_new("Порт для клиента №3");                        // Создаем label
  gtk_grid_attach(GTK_GRID(grid), label_port, 0, 4, 1, 1);                  // Помещаем label в grid

  gtk_entry_set_text(GTK_ENTRY(edit[2]), "Введите порт для клиента №3");     // Иницилизация начальной строки в поле ввода(edit[1])
  gtk_grid_attach(GTK_GRID(grid), edit[2], 1, 4, 1, 1);

  label_action[3] = gtk_label_new("Действие");            
  gtk_grid_attach(GTK_GRID(grid), label_action[3], 0, 5, 1, 1);

  combo[3] = gtk_combo_box_text_new(); // Создаем ComboBox
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo[3]), NULL, "Find Max");
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo[3]), NULL, "Find Min");
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo[3]), NULL, "Sort array");
  gtk_combo_box_set_active(GTK_COMBO_BOX(combo[3]), 0); 
  gtk_grid_attach(GTK_GRID(grid), combo[3], 1, 5, 1, 1); // добавляем ComboBox в окно

  button_start = gtk_button_new_with_label("Запуск");                             // Создаем button
  gtk_grid_attach(GTK_GRID(grid), button_start, 0, 6, 1, 1);                      // Помещаем button в grid
  g_signal_connect(GTK_BUTTON(button_start), "clicked", G_CALLBACK(click), NULL); // вызываем функцию click по нажатию button

  button_exit = gtk_button_new_with_label("Выход");                                           // создаем button1
  g_signal_connect(GTK_BUTTON(button_exit), "clicked", G_CALLBACK(gtk_main_quit), NULL);      // вызываем функцию gtk_main_quit по нажатию
  gtk_grid_attach(GTK_GRID(grid), button_exit, 1, 6, 1, 1);                                   // Помещаем button1 в grid

  gtk_window_set_position (GTK_WINDOW(window), GTK_WIN_POS_CENTER); // Задает начальную позицию окна при запуске

  gtk_widget_show_all(window);  // Показываем окно вместе с виджетами
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL); // выйти из программы по закрытию

  gtk_main();                                                 // Приложение переходит в цикл ожидания

  return 0;
}