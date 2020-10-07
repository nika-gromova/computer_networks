#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include "socketdata.h"

int main(void)
{
	int ClientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (ClientSocket < 0)
	{
		perror("socket error");
		exit(ClientSocket);
	}
	
	struct hostent* host = gethostbyname("localhost");
    if (!host)
    {
        perror("gethostbyname error");
        return -1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SPORT);
    server_addr.sin_addr = *((struct in_addr*) host->h_addr_list[0]);

	if (connect(ClientSocket, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect error");
        return -1;
    }

    printf("Input number: ");
	char message[BUFLEN];
	fgets(message, BUFLEN, stdin);
	sendto(ClientSocket, message, strlen(message), 0, (struct sockaddr*) &server_addr, sizeof(server_addr));
	close(ClientSocket);
	return 0;
}
