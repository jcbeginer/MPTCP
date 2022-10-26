/**
 * MPTCP Socket API Test App
 * File Recevier(Server)
 * 
 * @date	: 2022-10-25
 * @author  : Woo-sung(Netlab)
 * @ref	    : Ji-Hun(INSLAB)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

#include "../header/mptcp.h"

int get_fsize(FILE* file);
/**
 * 기존의 TCP Server는 { socket() -> bind() -> listen() -> accept() -> recv(), send() -> close() }순서로 흘러간다.
 * 여기서 TCP Socket을 MPTCP Socket으로 설정하기 위해서는 socket()과 bind()사이에 setsockopt()을 사용한다.
 **/
int main(int argc, char** argv)
{
	int PORT;
	//const char* FILE_NAME = "recv_file";
	const char* F_PATH;

	int server_sock, client_sock;
	struct sockaddr_in server_addr, client_addr;
	int len, addr_len, recv_len, ret;

	FILE *file;
	int fsize = 0, nsize = 0;
	char send_buff[1024];

	int enable = 1;

	if(argc != 3){
		fprintf(stderr, "usage: %s [port_number] [file_path]\n", argv[0]);
		return -1;
	}
	PORT = atoi(argv[1]);
	FILE_PATH = argv[2];

	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(server_sock < 0){
		perror("[server] socket() ");
		return -1;
	}

	/* setsockopt()함수와 MPTCP_ENABLED(=42)상수를 사용하여 MPTCP Socket으로 Setup */
	ret = setsockopt(server_sock, SOL_TCP, MPTCP_ENABLED, &enable, sizeof(int));
	if(ret < 0){
		perror("[server] setsockopt() ");
		return -1;
	}

	memset(&server_addr, 0x00, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);

	ret = bind(server_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));
	if(ret < 0){
		perror("[server] bind() ");
		return -1;
	}	

	ret = listen(server_sock, 5);
	if(ret < 0){
		perror("[server] listen() ");
		return -1;
	}

	addr_len = sizeof(struct sockaddr_in);
	client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_len);	
	if(client_sock < 0){
		perror("[server] accept() ");
		return -1;
	}
	printf("[server] connected client\n");

	file = fopen(FILE_PATH, "rb");
	if(file == NULL){
		perror("[server] fopen() ");
		return -1;
	}

	/*do{
		nsize = recv(client_sock, buffer, 1024, 0);
		fwrite(buffer, sizeof(char), nsize, file);
	}while(nsize!=0);*/
	fsize = get_fsize(file);

	printf("[server] sending file...(%s)\n", FILE_PATH); 
	while(nsize!=fsize){
		int fpsize = fread(send_buff, 1, 1024, file);
		nsize += fpsize;
		printf("[server] file size %dB | send to %dB\n", fsize, nsize);
		send(client_sock, send_buff, fpsize, 0);
	}

	printf("[server] sending file finished\n");
	
	fclose(file);
	close(client_sock);
	close(server_sock);

	return 0;
}

int get_fsize(FILE* file)
{
	int fsize;

	fseek(file, 0, SEEK_END);
	fsize=ftell(file);
	fseek(file, 0, SEEK_SET);	

	return fsize;
}
