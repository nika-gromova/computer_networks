#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "socketdata.h"


void calculate(int num, int base)
{
	if (num)
	{
		int tmp = num % base;
		num /= base;
		calculate(num, base);
		printf("%c", symbols[tmp]);
	}
}

void print_results(int num)
{
	printf("decimal: %d\n", num);
	printf("binary: ");
	calculate(num, 2);
	printf("\nhex: ");
	calculate(num, 16);
	printf("\noct: ");
	calculate(num, 8);
	printf("\n6th: ");
	calculate(num, 6);
	printf("\n");
}


int ListenSocket;

void close_socket(void)
{
	close(ListenSocket);
	printf("Socket closed\n");
}
	

void catch_sigint(int signum)
{
	close_socket();	
	exit(0);
}

int main (void)
{
	struct sockaddr_in ServerAddr, ClientAddr;
	char buffer[BUFLEN];
	ListenSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (ListenSocket < 0)
	{
		perror("socket error");
		exit(ListenSocket);
	}
	
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(SPORT);
    ServerAddr.sin_addr.s_addr = INADDR_ANY;
	
	if (bind(ListenSocket, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr)) < 0)
	{
		perror("bind error");
		close_socket();	
		exit(-1);
	}


	signal(SIGINT, catch_sigint);
	
	int len = sizeof(ClientAddr);

	printf("Server is ready to receive messages\n");
	
	while (1)
	{
		int recieved_len = recvfrom(ListenSocket, buffer, BUFLEN, 0, (struct sockaddr*)&ClientAddr, &len);
		if (recieved_len < 0)
		{
			perror("recvfrom error");
			close_socket();
			exit(-1);
		}
		buffer[recieved_len] = 0;
		print_results(atoi(buffer));
	}
	
	return 0;
}
