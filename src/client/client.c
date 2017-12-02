#include <stdio.h>      
#include <sys/socket.h> 
#include <arpa/inet.h>  
#include <stdlib.h>     
#include <string.h>     
#include <time.h>

#include "lib-trace.h"
#include "lib-func.h"

int create_socket(int sock, int port, char *IP) {

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
    
    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0) {
        trace_msg(ERR_MSG, "[%s], Client: connect() failed", __FUNCTION__);
        exit(1);
    }

    return sock;
}

int* recv_data(int sock, int* mas, int size_of_mas) {

    int bytesRecv;

    if ((bytesRecv = recv(sock, mas, size_of_mas, 0)) <= 0) {
        trace_msg(ERR_MSG, "[%s], Client: recv_data() failed", __FUNCTION__);
        exit(1);
    }

    return mas;
}

int recv_value(int sock, int value) {

    int bytesRecv;

    if ((bytesRecv = recv(sock, &value, sizeof(int), 0)) <= 0) {
        trace_msg(ERR_MSG, "[%s], Client: recv_value()) failed", __FUNCTION__);
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

void send_mass(int sock, int mass[], int size) {
     if (send(sock, mass, sizeof(int)*size, 0) != sizeof(int)*size) {
        trace_msg(ERR_MSG, "[%s], Client: send_value() failed", __FUNCTION__);
        exit(1);
    }
    else {
        trace_msg(DBG_MSG, "[%s], Client: value has sended.\n",__FUNCTION__);
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

    int sock = create_socket(sock, port, servIP);

    int n = recv_value(sock, n);
    int size_of_mas = sizeof(int) * n;
    int *mass;
    mass = malloc(size_of_mas);
    mass = recv_data(sock, mass, size_of_mas);

    for(int i = 0; i < n; i++) {
        trace_msg(DBG_MSG, "mass[%d] = %d\n", i, mass[i]);
    }
    
    trace_msg(DBG_MSG, "[%s], Client: massage from server accept.\n",__FUNCTION__);

    int action = recv_value(sock, action);
    trace_msg(DBG_MSG, "[%s], Client: action from server accept.\n",__FUNCTION__);
    
    if (action == SORT) {
        mass = sort(mass, n, action);
        send_mass(sock, mass, n);
    }
    else {
        int value = find_value(mass, n, action);
        send_value(sock, value);
    }
    
    return 0;

}
