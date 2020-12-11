#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
	

#define T 0
#define P 1
#define ADMIN 2

typedef struct{
	int p_row;
	int p_col;
	int t_row;
	int t_col;
	int who;
}DATA;

int start = 0;
	
void * send_msg(void * arg);
void * recv_msg(void * arg);
void error_handling(char * msg);
	
	
int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	pthread_t snd_thread, rcv_thread;
	void * thread_return;
	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	 }

	
	sock=socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
	  
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		error_handling("connect() error");
	
	pthread_create(&snd_thread, NULL, send_msg, (void*)&sock);
	pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);
	pthread_join(snd_thread, &thread_return);
	pthread_join(rcv_thread, &thread_return);
	close(sock);  
	return 0;
}
	
void * send_msg(void * arg)   // send thread main
{
	int sock=*((int*)arg);
	DATA data;
	int len;

	// data.p_col = 0;
	// data.p_row = 0;

	// while(1) {if(start) break;}

	data.who = P;

	while(1) 
	{
		write(sock, &data, sizeof(data));
		data.p_col++;
		data.p_row++;
		sleep(1);
	}
	return NULL;
}
	
void * recv_msg(void * arg)   // read thread main
{
	int sock=*((int*)arg);
	char result[500];
	int str_len;
	DATA data;

	while(1)
	{
		str_len = read(sock, &data, sizeof(data));
		start = 1;
		sprintf(result, "t_row: %d, t_col: %d\np_row: %d, p_col: %d", data.t_row, data.t_col, data.p_row, data.p_col);
		fputs(result, stdout);
		sleep(1);
	}
	return NULL;
}
	
void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
