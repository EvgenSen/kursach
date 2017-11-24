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
        trace_msg(ERR_MSG, "[%s], socket() failed", __FUNCTION__);
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
        trace_msg(ERR_MSG, "[%s], connect() failed", __FUNCTION__);
        exit(1);
    }

    return sock;
}

int* recv_data(int sock, int* mas, int size_of_mas) {

    int bytesRecv;

    if ((bytesRecv = recv(sock, mas, size_of_mas, 0)) <= 0) {
        trace_msg(ERR_MSG, "[%s], recv() failed", __FUNCTION__);
        exit(1);
    }

    return mas;
}


int main( int argc, char *argv[] ) {
                            
    if (argc != 3) {
        fprintf(stderr,"Usage: %s <Port> <IP-address>\n", argv[0]);
        exit(1);
    }

    unsigned short port;
    char *servIP = argv[2];

    port = atoi(argv[1]);

    int sock;
    int const n = 1000;
    int *mas;
    int size_of_mas = sizeof(int) * n;

    mas = malloc(size_of_mas);
    sock = create_socket(sock, port, servIP);
    mas = recv_data(sock, mas, size_of_mas);
    
    trace_msg(DBG_MSG, "[%s] Massage from server.\n",__FUNCTION__);

    return 0;

}
