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
#include <string.h>
#include <ctype.h>

#define MAXLINE 80

int main() {
	int listenfd = 0;
	int ret = 0;
	int connfd = 0;

	sockaddr_in srvAddr;
	sockaddr_in cltAddr;
	socklen_t addrLen;

	char pd[] = "2333\n";
	char fir[] = "Please input your password: \n";
	char buffer[BUFSIZ] = {0};
	char str[INET_ADDRSTRLEN];

	listenfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (listenfd == -1) {
		perror("socket: ");
		return -1;
	}
	memset(&srvAddr, 0, sizeof(sockaddr_in));
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	srvAddr.sin_port = htons(8080);

	ret = bind(listenfd, (struct sockaddr*) &srvAddr, sizeof(srvAddr));
	if (ret == -1) {
		perror("bind: ");
		return -1;
	}

	int flag = 0; int n = 0;
	printf("wait for connect...\n");
	addrLen = sizeof(cltAddr);
		
	while (true) {
		while (!flag) {
			n = recvfrom(listenfd, buffer, MAXLINE, 0, \
				(struct sockaddr*) &cltAddr, &addrLen);
			if (n == -1) {
				printf("recvfrom error\n");
			}
			if (strcmp(pd, buffer)) {
				n = 41;
				printf("Warning! An illegal access!\n");
				strcpy(buffer, "Wrong! Please input your password again:\n");
				flag = 0;
			} else {
				n = 7;
				strcpy(buffer, "Right!\n");
				flag = 1;
			}
			n = sendto(listenfd, buffer, n, 0, \
		    	(struct sockaddr *) &cltAddr, sizeof(cltAddr));
			if (n == -1) {
				printf("sendto error\n");
			}
			memset(buffer, 0, sizeof(buffer));
		}
		n = recvfrom(listenfd, buffer, MAXLINE, 0, \
			(struct sockaddr*) &cltAddr, &addrLen);
		if (n == -1) {
            printf("recvfrom error\n");
        }
        printf("receive data from %s whose port is %d\n", \
        	(char*)inet_ntop(AF_INET, &cltAddr.sin_addr, str, sizeof(str)), \
            ntohs(cltAddr.sin_port));
        for(int i = 0; i<n; i++){
            buffer[i] = toupper(buffer[i]);
        }
        if (!strcmp(buffer, "Q\n")) {
        	break;
        }
        strcat(buffer, fir);
        n = strlen(buffer);
        n = sendto(listenfd, buffer, n, 0, \
        	(struct sockaddr *) &cltAddr, sizeof(cltAddr));
        if (n == -1) {
        	printf("sendto error\n");
        }
        memset(buffer, 0, sizeof(buffer));
        flag = 0;
	}
	n = 4;
	strcpy(buffer, "Bye\n");
	n = sendto(listenfd, buffer, n, 0, \
        (struct sockaddr *) &cltAddr, sizeof(cltAddr));
    if (n == -1) {
    	printf("sendto error\n");
    }

	close(listenfd);
	return 0;
}