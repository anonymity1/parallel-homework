#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 80

int main(int argc, char* argv[]) {
    int listenfd = 0;
    int ret = 0;
    sockaddr_in srvAddr;
    socklen_t addrLen;
    
    char buffer[MAXLINE] = {0};

    if (argc == 1) {
        perror("please input the ip address that you want to connect\n");
        return -1;
    }

    listenfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (listenfd == -1) {
        perror("socket:");
        return -1;
    }
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = inet_addr(argv[1]);
    srvAddr.sin_port = htons(8080);

    printf("Please input your password: \n");
    while (fgets(buffer, MAXLINE, stdin) != NULL) {
        int n = sendto(listenfd, buffer, strlen(buffer), 0, \
            (struct sockaddr*) &srvAddr, sizeof(srvAddr));
        if (n == -1) {
            printf("sebdto err\n");
        }
        n = recvfrom(listenfd, buffer, MAXLINE, 0, NULL, 0);
        if (n == -1) {
            printf("recvfrom err\n");
        }
        if (!strcmp(buffer, "Bye\n")) {
            break;
        }
        write(STDOUT_FILENO, buffer, n);
        memset(buffer, 0, sizeof(buffer));
    }
    return 0;
}