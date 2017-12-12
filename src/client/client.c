#include <stdio.h>      
#include <sys/socket.h> 
#include <arpa/inet.h>  
#include <stdlib.h>     
#include <string.h>     
#include <time.h>
#include <unistd.h>

#include "lib-trace.h"
#include "lib-func.h"

int create_socket(int port, char *IP) {

    int sock;

    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        trace_msg(ERR_MSG, "[%s], Client: socket() failed", __FUNCTION__);
        exit(1);
    }


    struct sockaddr_in echoServAddr;
    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr(IP);    /* Any incoming interface */
    echoServAddr.sin_port = htons(port);              /* Local port */

    /* Bind to the local address */
    
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

int* recv_data(int sock, int* mass, int size) {

    int bytesRecv;

    while(bytesRecv != (size*sizeof(int))) {
        if ((bytesRecv = recv(sock, mass, sizeof(int)*size, 0)) <= 0) {
            trace_msg(ERR_MSG, "[%s], Client: recv_data() failed", __FUNCTION__);
            exit(1);
        }
    }
    return mass;
}

int recv_value(int sock) {

    int value;
    int bytesRecv;

    if ((bytesRecv = recv(sock, &value, sizeof(int), 0)) <= 0) {
        trace_msg(ERR_MSG, "[%s], Client: recv_value()) failed", __FUNCTION__);
        trace_msg(ERR_MSG, "[%s], Server: bytesRecv = %d", __FUNCTION__, bytesRecv);
        exit(1);
    }

    return value;
}

void send_value(int sock, int value) {

    if (send(sock, &value, sizeof(int), 0) != sizeof(int)) {
        trace_msg(ERR_MSG, "[%s], Client: send_value() failed", __FUNCTION__);
        exit(1);
    }
    else {
        trace_msg(DBG_MSG, "[%s], Client: value has sended.\n",__FUNCTION__);
    }
}

void send_mass(int sock, int* mass, int size) {

     if (send(sock, mass, sizeof(int)*size, 0) != sizeof(int)*size) {
        trace_msg(ERR_MSG, "[%s], Client: send_mass() failed", __FUNCTION__);
        exit(1);
    }
    else {
        trace_msg(DBG_MSG, "[%s], Client: value has sended.\n",__FUNCTION__);
        // trace_msg(ERR_MSG, "[%s], Server: bytesRecv = %d", __FUNCTION__, bytesRecv);
    }
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

int check_ip(char *addr) {
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

int main( int argc, char *argv[] ) {
                            
    if (argc != 3) {
        fprintf(stderr,"Usage: %s <Port> <IP-address>\n", argv[0]);
        exit(1);
    }

    int port;

    port = atoi(argv[1]);

    if(check_port(port))
        return 0;

    char *servIP = argv[2];

    if(check_ip(servIP))
        return 0;

    int sock = create_socket(port, servIP);
    int size_of_mas;
    size_of_mas = recv_value(sock);
    trace_msg(DBG_MSG, "[%s], Client: size_of_mas %d from server accept.\n", __FUNCTION__,size_of_mas);
    int *mass;
    mass = malloc(sizeof(int)*size_of_mas);
    mass = recv_data(sock, mass, size_of_mas);
    
/* Очень много сообщений в консоль
    for(int i = 0; i < size_of_mas; i++) {
        trace_msg(DBG_MSG, "mass[%d] = %d\n", i, mass[i]);
    }
*/
    trace_msg(DBG_MSG, "[%s], Client: message from server accept.\n",__FUNCTION__);
    int action;
    int value;

    action = recv_value(sock);
    trace_msg(DBG_MSG, "[%s], Client: action %d from server accept.\n", __FUNCTION__,action);
    
    switch (action) { 
    case 0:
        // send_value(sock, point->action);
        value = find_value(mass, size_of_mas, action);
        send_value(sock, value);
        trace_msg(DBG_MSG, "[%s], Client: action - find Max value in array (%d) \n",__FUNCTION__, value);
        break;
    case 1:
        value = find_value(mass, size_of_mas, action);
        send_value(sock, value);
        trace_msg(DBG_MSG, "[%s], Client: action - find Min value in array (%d) \n",__FUNCTION__, value);
        break;
    case 2:
        mass = sort(mass, size_of_mas, action);
        send_mass(sock, mass, size_of_mas);
        trace_msg(DBG_MSG, "[%s], Client: action - Sort array\n", __FUNCTION__);
        break;
    default:
        trace_msg(ERR_MSG, "[%s], Client: action - Unknown action \n",__FUNCTION__);
        break;
    }
    close(sock);
    free(mass);
    return 0;
}
