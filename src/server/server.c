#define _GNU_SOURCE

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
  
#include "lib-trace.h"
#include "lib-func.h"

#define FILE_NAME "input.txt"
#define N 3

static GtkWidget *edit[N];    // Массив для полей ввода
static GtkWidget *combo[N];      // ComboBox для списка

unsigned flag = 0; /* блокируем повторный запуск сервера */

struct prot_cl {
  int port_cl;
  int* mass_proc_cl;
  int size_of_mass_cl;
  int action_cl;
  int id; 
};

struct prot_serv {
  int** mass_proc_s;
  int size_of_mass_s[N];
  int action_s[N];
};

/* Возвращает милисекунды */
int get_time ()
{
  struct timeval tv;
  gettimeofday(&tv, 0);
  return (int)tv.tv_usec / 1000;
}

void print_time_diff (struct timeval tv_start, struct timeval tv_end, char * app)
{
  int ms = (int)(tv_end.tv_usec - tv_start.tv_usec) / 1000;
  int s  = (int)(tv_end.tv_sec - tv_start.tv_sec);
  if (ms < 0)
    ms = 1 - ms;
  trace_msg(DBG_MSG, "[%s] Time work of %s: %d ms \n",__FUNCTION__, app, s * 1000 + ms);
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

int check_sameport(struct prot_cl *check, int size) {

  for(int i = 0; i < size - 1; i++) {
    if(check[i].port_cl == check[i+1].port_cl) {
      trace_msg(ERR_MSG, "[%s] Same ports '%d' and  '%d', choise other number of port", __FUNCTION__,check[i].port_cl,check[i+1].port_cl);
      return 1;
    }
  }
  return 0;
}

int create_socket(int sock, int port_serv) {


  if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
    trace_msg(ERR_MSG, "[%s] Server: socket() failed", __FUNCTION__);
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
    trace_msg(ERR_MSG, "[%s] Server: bind() failed", __FUNCTION__);
    exit(1);
  }

  if (listen(sock, 5) < 0) {
    trace_msg(ERR_MSG, "[%s] Server: listen() failed", __FUNCTION__);
    exit(1);
  }

  struct sockaddr_in echoClntAddr; /* Client address */
  unsigned int clntLen;            /* Length of client address data structure */

  /* Set the size of the in-out parameter */
  clntLen = sizeof(echoClntAddr);

  /* Mark the socket so it will listen for incoming connections */
  if ((sock = accept(sock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0) {
    trace_msg(ERR_MSG, "[%s] Server: accept() failed", __FUNCTION__);
    exit(1);
  }
  return sock;
}

void send_data(int sock, int *data, int size_of_data) {

  if (send(sock, data, sizeof(int)*size_of_data, 0) != sizeof(int)*size_of_data) {
    trace_msg(ERR_MSG, "[%s] Server: send() failed", __FUNCTION__);
    exit(1);
  }
  else {
    trace_msg(DBG_MSG, "[%s] Server: massive has sended.\n",__FUNCTION__);
  }
}

void send_value(int sock, int value) {

  if (send(sock, &value, sizeof(int), 0) != sizeof(int)) {
    trace_msg(ERR_MSG, "[%s] Server: send() failed", __FUNCTION__);
    exit(1);
  }
  else {
    trace_msg(DBG_MSG, "[%s] Server: value has sended.\n",__FUNCTION__);
  }
}

int recv_value(int sock, int value_cl) {

  int bytesRecv;

  if ((bytesRecv = recv(sock, &value_cl, sizeof(int), 0)) <= 0) {
    trace_msg(ERR_MSG, "[%s] Server: recv_value()) failed", __FUNCTION__);
    exit(1);
  } else {
    trace_msg(DBG_MSG, "[%s] Server: value has recv.\n",__FUNCTION__);
  }
  return value_cl;
}

int* recv_mass(int sock, int* mass, int size_of_mas) {

  int bytesRecv;

  if ((bytesRecv = recv(sock, mass, sizeof(int)*size_of_mas, 0)) <= 0) {
    trace_msg(ERR_MSG, "[%s] Server: recv_mass()) failed", __FUNCTION__);
    exit(1);
  } else {
    trace_msg(DBG_MSG, "[%s] Server: mas has recv.\n",__FUNCTION__);
  }
  
  return mass;
}

void *client(void *threadArgs)
{
  struct prot_cl *point = (struct prot_cl*)threadArgs;
  int value_min = 0;
  int value_max = 0;
  struct timeval start_time, end_time;
  gettimeofday(&start_time, 0);
  
  int sock;
  
  sock = create_socket(sock, point->port_cl);
  trace_msg(DBG_MSG, "[%s] Port: %d",__FUNCTION__, point->port_cl);
  send_value(sock, point->size_of_mass_cl);
  trace_msg(DBG_MSG, "[%s] Size: %d",__FUNCTION__, point->size_of_mass_cl);
  trace_msg(DBG_MSG, "[%s] Action: %d",__FUNCTION__, point->action_cl);
  trace_msg(DBG_MSG, "[%s] Id: %d",__FUNCTION__, point->id);
  send_data(sock, point->mass_proc_cl, point->size_of_mass_cl);
  trace_msg(DBG_MSG,"[%s] Client %d:\n", __FUNCTION__, point->id);
  
  switch (point->action_cl)
  { 
    case 0:
      send_value(sock, point->action_cl);
      value_max = recv_value(sock, value_max);
      trace_msg(DBG_MSG, "[%s] Client %d: action - find Max value in array (%d) \n",__FUNCTION__,point->id, value_max);
      break;
    case 1:
      send_value(sock, point->action_cl);
      value_min = recv_value(sock, value_min);
      trace_msg(DBG_MSG, "[%s] Client %d: action - find Min value in array (%d) \n",__FUNCTION__,point->id, value_min);
      break;
    case 2:
      send_value(sock, point->action_cl);
      point->mass_proc_cl = recv_mass(sock, point->mass_proc_cl, point->size_of_mass_cl);
      trace_msg(DBG_MSG, "[%s] Client %d: action - Sort array\n", __FUNCTION__,point->id);
      break;
    default:
      trace_msg(ERR_MSG, "[%s] Client %d: action - Unknown action \n",__FUNCTION__,point->id);
      break;
  }
  close(sock);
  gettimeofday(&end_time, 0);
  print_time_diff(start_time,end_time,"Client");
}

void *server(void *threadArgs) {

  struct prot_serv *point = (struct prot_serv*)threadArgs;
  int value_max; 
  int value_min;
  struct timeval start_time, end_time;
  gettimeofday(&start_time, 0);

  for(int i = 0; i < N; i++) {
    trace_msg(DBG_MSG, "[%s] Size: %d",__FUNCTION__, point->size_of_mass_s[i]);
    trace_msg(DBG_MSG, "[%s] Action: %d",__FUNCTION__, point->action_s[i]);

    switch (point->action_s[i])
    {
      case 0:
        value_max = find_value(point->mass_proc_s[i], point->size_of_mass_s[i], point->action_s[i]);
        trace_msg(DBG_MSG, "[%s] Server: action - find Max value in array (%d) \n",__FUNCTION__, value_max);
        break;
      case 1:
        value_min = find_value(point->mass_proc_s[i], point->size_of_mass_s[i], point->action_s[i]);
        trace_msg(DBG_MSG, "[%s] Server: action - find Min value in array (%d) \n",__FUNCTION__, value_min);
        break;
      case 2:       
        point->mass_proc_s[i] = sort(point->mass_proc_s[i], point->size_of_mass_s[i], point->action_s[i]);
        trace_msg(DBG_MSG, "[%s] Server: action - Sort array\n", __FUNCTION__);
        break;
      default:
        trace_msg(ERR_MSG, "[%s] Server: action - Unknown action \n",__FUNCTION__);
        break;
    }  
  }
  gettimeofday(&end_time, 0);
  print_time_diff(start_time,end_time,"Server");
}

/* Обрабатываем входные данные и запускаем работу */
void click(GtkWidget *widget, GtkWidget *entry) {

  if (flag) {
    trace_msg(ERR_MSG, "[%s] Server is already running \n",__FUNCTION__);
    return;
  }
  flag = 1;

  struct prot_cl workers[N];
  struct prot_serv host;  
  // int PORT[n];

  for(int i = 0; i < N; i++) {
    workers[i].port_cl = atoi((gchar*)gtk_entry_get_text(GTK_ENTRY(edit[i])));
    workers[i].action_cl = gtk_combo_box_get_active(GTK_COMBO_BOX(combo[i]));
    workers[i].id = i;
    host.action_s[i] = workers[i].action_cl;
  }

  for(int i = 0; i < N; i++) {
    if (check_port(workers[i].port_cl))
      return;
  }

  if (check_sameport(workers, N))
    return;

  int start_t_cl, stop_t_cl;
  int *mass = NULL;
  int value_s;
  int value_cl;
  
  int k = read_array_from_file(&mass);

  /*Start Share mass */
  int i = 0;

  host.mass_proc_s = malloc(sizeof(int*)*N);

  for(int j = 0; j < N; j++) {
    workers[j].mass_proc_cl = malloc(sizeof(int)*(k/N));
    host.mass_proc_s[j] = malloc(sizeof(int)*(k/N));
    workers[j].size_of_mass_cl = (k/N);
    host.size_of_mass_s[j] = workers[j].size_of_mass_cl;
    for(int l = 0; l < k/N; l++) {
      workers[j].mass_proc_cl[l] = mass[i];
      host.mass_proc_s[j][l] = mass[i];  
      i++;
    }
  }
  
  /*Start thread */
  pthread_t pt_server, clients[N];

  for(int i = 0; i < N; i++) {
    pthread_create(&clients[i], NULL, client, &workers[i]);
  }
  pthread_create(&pt_server, NULL, server, &host);
  /* end tread */
  gtk_main();
  trace_msg(DBG_MSG, "[%s] Server stopped \n",__FUNCTION__);
  flag = 0;
}

int main( int argc, char *argv[] ) {
  /* Описываем виджеты GTK */
  GtkWidget *label_port;
  GtkWidget *label_action[N];
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

  label_action[2] = gtk_label_new("Действие");            
  gtk_grid_attach(GTK_GRID(grid), label_action[2], 0, 5, 1, 1);

  combo[2] = gtk_combo_box_text_new(); // Создаем ComboBox
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo[2]), NULL, "Find Max");
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo[2]), NULL, "Find Min");
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo[2]), NULL, "Sort array");
  gtk_combo_box_set_active(GTK_COMBO_BOX(combo[2]), 0); 
  gtk_grid_attach(GTK_GRID(grid), combo[2], 1, 5, 1, 1); // добавляем ComboBox в окно

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