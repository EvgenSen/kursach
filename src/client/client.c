#include <stdio.h>      
#include <sys/socket.h> 
#include <arpa/inet.h>  
#include <stdlib.h>     
#include <string.h>     
#include <time.h>
#include <unistd.h>
#include <errno.h>

#include "lib-trace.h"
#include "lib-func.h"

int create_socket(int port) {

    int sock;

    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        trace_msg(ERR_MSG, "[Line:%4d] Client: socket() failed", __LINE__);
        exit(1);
    }


    struct sockaddr_in echoServAddr;
    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(port);              /* Local port */

    /* Bind to the local address */
    
    /* Bind to the local address */
  if (bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0) {
    trace_msg(ERR_MSG, "[Line:%4d] Server: bind() failed", __LINE__);
    exit(1);
  }

  if (listen(sock, 5) < 0) {
    trace_msg(ERR_MSG, "[Line:%4d] Server: listen() failed", __LINE__);
    exit(1);
  }

  struct sockaddr_in echoClntAddr; /* Client address */
  unsigned int clntLen;            /* Length of client address data structure */

  /* Set the size of the in-out parameter */
  clntLen = sizeof(echoClntAddr);

  /* Mark the socket so it will listen for incoming connections */
  if ((sock = accept(sock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0) {
    trace_msg(ERR_MSG, "[Line:%4d] Server: accept() failed", __LINE__);
    exit(1);
  }

    return sock;
}
/*
int* recv_data(int sock, int* mass, int size) {

    int bytesRecv;

    while(bytesRecv != (size*sizeof(int))) {
        if ((bytesRecv = recv(sock, mass, sizeof(int)*size, 0)) <= 0) {
            trace_msg(ERR_MSG, "[Line:%4d] Client: recv_data() failed", __LINE__);
            exit(1);
        }
    }
    return mass;
}
*/
int* recv_data(int sock, int* mass, int size) {

  int bytesRecv = 0;
  int bytesRecvTMP = 0;
  int shift = 0;

  while(bytesRecv != (size*sizeof(int))) 
  {
    bytesRecvTMP = recv(sock, &mass[shift], sizeof(int)*size - bytesRecv, 0);

    if (bytesRecvTMP == 0)
    {
      trace_msg(ERR_MSG, "[Line:%4d] Client: Socket closed before enough data received", __LINE__);
      break;
    }
    else if (bytesRecvTMP < 0)
    {
      trace_msg(ERR_MSG, "[Line:%4d] Client: recv_data() failed, \"%s\"", __LINE__, strerror(errno));
      break;
    }
    else if (bytesRecvTMP > size*sizeof(int))
    {
      // больше не должно происходить
      trace_msg(ERR_MSG, "[Line:%4d] Client: recv_data() failed, received extra data (%d/%d)", __LINE__, bytesRecv, size*sizeof(int));
    }
    bytesRecv += bytesRecvTMP;
    shift = bytesRecv / sizeof(int);
    trace_msg(DBG_MSG, "[Line:%4d] Client: Data received (%d/%d)(%d/%d)",__LINE__, bytesRecv, size*sizeof(int),
                                                                                   shift, size);
  }
  return mass;
}

int recv_value(int sock) {

    int value;
    int bytesRecv;

    if ((bytesRecv = recv(sock, &value, sizeof(int), 0)) <= 0) {
      trace_msg(ERR_MSG, "[Line:%4d] Client: recv_value()) failed", __LINE__);
      trace_msg(ERR_MSG, "[Line:%4d] Server: bytesRecv = %d", __LINE__, bytesRecv);
      exit(1);
    } else {
      trace_msg(DBG_MSG, "[Line:%4d] Client: Value received (%d)",__LINE__, value);
    }

    return value;
}

void send_value(int sock, int value) {

    if (send(sock, &value, sizeof(int), 0) != sizeof(int)) {
        trace_msg(ERR_MSG, "[Line:%4d] Client: send_value() failed", __LINE__);
        exit(1);
    }
    else {
        trace_msg(DBG_MSG, "[Line:%4d] Client: Value (%d) has sended.\n",__LINE__, value);
    }
}

void send_mass(int sock, int* mass, int size) {

     if (send(sock, mass, sizeof(int)*size, 0) != sizeof(int)*size) {
        trace_msg(ERR_MSG, "[Line:%4d] Client: send_mass() failed", __LINE__);
        exit(1);
    }
    else {
        trace_msg(DBG_MSG, "[Line:%4d] Client: Massive has sended. Size %d \n",__LINE__, size);
        // trace_msg(ERR_MSG, "[Line:%4d] Server: bytesRecv = %d", __LINE__, bytesRecv);
    }
}

int check_port(int port)
{
  if (port > 65536 || port <= 0)
  {
    trace_msg(ERR_MSG, "[Line:%4d] Incorrect port value '%d' ", __LINE__,port);
    return 1;
  }
  return 0;
}

int main( int argc, char *argv[] ) {
                            
    if (argc != 2) {
        fprintf(stderr,"Usage: %s <Port>\n", argv[0]);
        exit(1);
    }

    int port;

    port = atoi(argv[1]);

    if(check_port(port))
        return 0;

    int sock = create_socket(port);
    int action = -1;
    action = recv_value(sock);
    trace_msg(DBG_MSG, "[Line:%4d] Client: action %d from server accept.\n", __LINE__,action);

    int size_of_mas = 0;
    size_of_mas = recv_value(sock);
    trace_msg(DBG_MSG, "[Line:%4d] Client: size_of_mass %d from server accept.\n", __LINE__,size_of_mas);
    
    int *mass = NULL;
    mass = malloc(sizeof(int)*size_of_mas);
    mass = recv_data(sock, mass, size_of_mas);
    trace_msg(DBG_MSG, "[Line:%4d] Client: array from server accept.\n", __LINE__);
/* Очень много сообщений в консоль
    for(int i = 0; i < size_of_mas; i++) {
        trace_msg(DBG_MSG, "mass[%d] = %d\n", i, mass[i]);
    }
*/
    int value = 0;
    switch (action) { 
    case 0:
        // send_value(sock, point->action);
        value = find_value(mass, size_of_mas, action);
        send_value(sock, value);
        trace_msg(DBG_MSG, "[Line:%4d] Client: action - find Max value in array (%d) \n",__LINE__, value);
        break;
    case 1:
        value = find_value(mass, size_of_mas, action);
        send_value(sock, value);
        trace_msg(DBG_MSG, "[Line:%4d] Client: action - find Min value in array (%d) \n",__LINE__, value);
        break;
    case 2:
        mass = sort(mass, size_of_mas, action);
        send_mass(sock, mass, size_of_mas);
        trace_msg(DBG_MSG, "[Line:%4d] Client: action - Sort array\n", __LINE__);
        break;
    default:
        trace_msg(ERR_MSG, "[Line:%4d] Client: action - Unknown action \n",__LINE__);
        break;
    }
    close(sock);
    free(mass);
    return 0;
}
