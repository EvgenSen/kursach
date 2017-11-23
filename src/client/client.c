#include <stdio.h>      
#include <sys/socket.h> 
#include <arpa/inet.h>  
#include <stdlib.h>     
#include <string.h>     
#include <unistd.h>     
#include <time.h>

int main( int argc, char *argv[] ) {
                            
    if (argc != 3) {
        fprintf(stderr,"Usage: %s <Port> <IP-address>\n", argv[0]);
        exit(1);
    }

    unsigned short port;
    char *servIP = argv[2];

    port = atoi(argv[1]);

    int sock;

    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        perror("socket() failed");
        exit(1);
    }

    struct sockaddr_in echoServAddr;

    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);    /* Any incoming interface */
    echoServAddr.sin_port = htons(port);              /* Local port */

    /* Bind to the local address */
    
    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0) {
        perror("connect() failed");
        exit(1);
    }

    int const n = 500;

    int mas[n];
    int size_of_mas = sizeof(int) * n;
    int bytesRecv;

    if ((bytesRecv = recv(sock, mas, size_of_mas, 0)) <= 0) {
        perror("recv() failed");
        exit(1);
    }

    printf("$@#@\n");
    int i;

    for ( i = 0; i < n; i++) {
        printf("mas[%d] = %d\n", i, mas[i]);
    }
    printf("\n");
    printf("<<< Пришел ответ от сервера >>>\n");

    return 0;

}
