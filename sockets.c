#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "sockets.h"

 int createSocket(int PORT)
 {
    // Address
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORT);

    // Create the socket for UDP
    int socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);
    if (socketDescriptor < 0) {
        error("Error socketDescriptor initialization");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to the port (PORT) that we specify
    if (!bind (socketDescriptor, (struct sockaddr*) &sin, sizeof(sin))) {
        perror("Error bind");
        exit(EXIT_FAILURE);
    }

    return socketDescriptor;
 }

void closeSocket(int socketDescriptor)
{
    // Close 
    close(socketDescriptor);

    return;
}
 
 
 