#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define MAXLINE  511
#define MAX_SOCK 1024 

void error_handling(char *message);

char *EXIT = "exit";	
char *START = "서버와 연결성공! 환영합니다 \n";

int max_fdp;				
int user_num = 0;			
int chat_num = 0;			
int clisock_list[MAX_SOCK];		
char ip_list[MAX_SOCK][20];		
int listen_sock;			

							
void add_Client(int s, struct sockaddr_in *newcliaddr);
int get_max();				
void rm_Client(int s);
int tcp_listen(int host, int port, int backlog);
void errquit(char *mesg) { perror(mesg); exit(1); }

time_t ct;
struct tm tm;

//스레드 사용방법 알려주는 메서드
void *thread_function(void *arg) { 
	int i;
	printf("서버 지원 명령어 목록: help, user_num, chat_num, ip_list, exit \n");
	while (1) {
		char bufmsg[MAXLINE + 2];
		fprintf(stderr, "\033[1;32m"); 
		printf("server>"); 
		fgets(bufmsg, MAXLINE, stdin); 
		if (!strcmp(bufmsg, "\n")) continue; 
		
		else if (!strcmp(bufmsg, "help\n"))  
			printf("help, user_num, chat_num, ip_list, exit\n");

		else if (!strcmp(bufmsg, "user_num\n"))
			printf("현재 참가자 수 = %d\n", user_num);
		
		else if (!strcmp(bufmsg, "chat_num\n"))
			printf("지금까지 오간 대화의 수 = %d\n", chat_num);

		else if (!strcmp(bufmsg, "ip_list\n")) 
			for (i = 0; i < user_num; i++)
				printf("%s\n", ip_list[i]);
		
		else if (!strcmp(bufmsg, "exit\n"))
			exit(0);
		else 
			printf("해당 명령어가 없습니다.help를 참조하세요.\n");
	}
}

int main(int argc, char *argv[]) {
	struct sockaddr_in cliaddr;
	char buf[MAXLINE + 1]; 
	int i, j, nbyte, accp_sock, addrlen = sizeof(struct
		sockaddr_in);
	fd_set read_fds;
	pthread_t a_thread;

	if (argc != 2) {
		printf("사용법 :%s port\n", argv[0]);
		exit(0);
	}
	listen_sock = tcp_listen(INADDR_ANY, atoi(argv[1]), 5);
	pthread_create(&a_thread, NULL, thread_function, (void *)NULL);
	while (1) {
		FD_ZERO(&read_fds);
		FD_SET(listen_sock, &read_fds);
		for (i = 0; i < user_num; i++)
			FD_SET(clisock_list[i], &read_fds);

		max_fdp = get_max() + 1;	
		if (select(max_fdp, &read_fds, NULL, NULL, NULL) < 0)
			errquit("select fail");

		if (FD_ISSET(listen_sock, &read_fds)) {
			accp_sock = accept(listen_sock,
				(struct sockaddr*)&cliaddr, &addrlen);
			if (accp_sock == -1) errquit("accept fail");
			add_Client(accp_sock, &cliaddr);
			send(accp_sock, START, strlen(START), 0);
			ct = time(NULL);			
			tm = *localtime(&ct);
			write(1, "\033[0G", 4);	
			printf("[%02d:%02d:%02d]", tm.tm_hour, tm.tm_min, tm.tm_sec);
			fprintf(stderr, "\033[33m");
			printf("사용자 1명 추가. 현재 참가자 수 = %d\n", user_num);
			fprintf(stderr, "\033[32m");
			fprintf(stderr, "server>"); 
		}

		for (i = 0; i < user_num; i++) {
			if (FD_ISSET(clisock_list[i], &read_fds)) {
				chat_num++;				
				nbyte = recv(clisock_list[i], buf, MAXLINE, 0);
				if (nbyte <= 0) {
					rm_Client(i);	
					continue;
				}
				buf[nbyte] = 0;
				if (strstr(buf, EXIT) != NULL) {
					rm_Client(i);	
					continue;
				}
				for (j = 0; j < user_num; j++)
					send(clisock_list[j], buf, nbyte, 0);
				printf("\033[0G");	
				fprintf(stderr, "\033[97m");
				printf("%s", buf);	
				fprintf(stderr, "\033[32m");
				fprintf(stderr, "server>"); 
			}
		}

	}  

	return 0;
}

//새로운 채팅 참여자 처리 메서드
void add_Client(int s, struct sockaddr_in *newcliaddr) {
	char buf[20];
	inet_ntop(AF_INET, &newcliaddr->sin_addr, buf, sizeof(buf));
	write(1, "\033[0G", 4);		
	fprintf(stderr, "\033[33m");	
	printf("new client: %s\n", buf);
	clisock_list[user_num] = s;
	strcpy(ip_list[user_num], buf);
	user_num++; 
}

//탈퇴 참여자 처리 메서드
void rm_Client(int s) {
	close(clisock_list[s]);
	if (s != user_num - 1) { 
		clisock_list[s] = clisock_list[user_num - 1];
		strcpy(ip_list[s], ip_list[user_num - 1]);
	}
	user_num--; 
	ct = time(NULL);	
	tm = *localtime(&ct);
	write(1, "\033[0G", 4);	
	fprintf(stderr, "\033[33m");
	printf("[%02d:%02d:%02d]", tm.tm_hour, tm.tm_min, tm.tm_sec);
	printf("채팅 참가자 1명 탈퇴. 현재 참가자 수 = %d\n", user_num);
	fprintf(stderr, "\033[32m");
	fprintf(stderr, "server>"); 
}

//소켓 번호 최대 찾기
int get_max() {
	int max = listen_sock;
	int i;
	for (i = 0; i < user_num; i++)
		if (clisock_list[i] > max)
			max = clisock_list[i];
	return max;
}

//소켓 생성 및 파일 읽
int  tcp_listen(int host, int port, int backlog) {
	int sd;
	struct sockaddr_in servaddr;

	sd = socket(AF_INET, SOCK_STREAM, 0);
	if (sd == -1) {
		error_handling("socket() error");
	}
	bzero((char *)&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(host);
	servaddr.sin_port = htons(port);
	if (bind(sd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		error_handling("bind() error");  
	}
	listen(sd, backlog);
	return sd;
}

//에러 처리
void error_handling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

