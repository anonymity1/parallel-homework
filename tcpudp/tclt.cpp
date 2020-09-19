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
#include <ctype.h>

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

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
        perror("socket:");
        return -1;
    }
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = inet_addr(argv[1]);
    srvAddr.sin_port = htons(8080);

    ret = connect(listenfd, (struct sockaddr*) &srvAddr, sizeof(srvAddr));
    if (ret == -1) {
        perror("connect:");
        return -1;
    }

    printf("Please input your password: \n");
    while (fgets(buffer, MAXLINE, stdin) != NULL) {
        int n = send(listenfd, buffer, strlen(buffer), 0);
        if (n == -1) {
            printf("send err\n");
        }
        n = recv(listenfd, buffer, MAXLINE, 0);
        if (n == -1) {
            printf("recv err\n");
        }
        if (!strcmp(buffer, "Bye\n")) {
            break;
        }
        write(STDOUT_FILENO, buffer, n);
        memset(buffer, 0, sizeof(buffer));
    }

    return 0;
}