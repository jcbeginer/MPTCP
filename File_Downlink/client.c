/**
 * MPTCP Socket API Test App
 * File Sender(Client) - downlink
 * 
 * @date	: 2022-10-25)
 * @author : Woo-sung(NETLAB)
 * @ref	   : Ji-Hun(INSLAB)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#include "../header/mptcp.h"

int get_fsize(FILE* file);

/**
 * 기존의 TCP Client는 { socket() -> connect() -> recv(), send() -> close() }순서로 흘러간다.
 * 여기서 TCP Socket을 MPTCP Socket으로 설정하기 위해서는 socket()과 connect()사이에 setsockopt()을 사용한다.
 **/
int main(int argc, char** argv)
{
	char* ADDR;
	int PORT;
	//char* FILE_PATH;
    const char* FILE_NAME = "recv_file";


	int sock;
	struct sockaddr_in addr;
	int ret;

	FILE* file;
	//char send_buff[1024] = { '\0', };
    char buffer[1024];
	int fsize = 0, nsize = 0;

	int enable = 1;
	char* path_manager = "fullmesh";
//Usage : ./client 52.79.185.248 1981
	if(argc != 3){
		fprintf(stderr, "usage: %s [host_address] [port_number]\n", argv[0]);
		return -1;
	}
	ADDR = argv[1];
	PORT = atoi(argv[2]);
	//FILE_PATH = argv[3];

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		perror("[client] socket() ");
		return -1;
	}

	/* setsockopt()함수와 MPTCP_ENABLED(=42)상수를 사용하여 MPTCP Socket으로 Setup */
	ret = setsockopt(sock, SOL_TCP, MPTCP_ENABLED, &enable, sizeof(int));
	if(ret < 0){
		perror("[server] setsockopt(MPTCP_ENABLED) ");
		return -1;
	}

	memset(&addr, 0x00, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ADDR);
	addr.sin_port = htons(PORT);

	ret = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
	if(ret < 0){
		perror("[client] connect() ");
		return -1;
	}
	printf("[client] connected\n");
    /*
	file = fopen(FILE_PATH, "rb");
	if(file == NULL){
		perror("[client] fopen() ");
		return -1;
	}

	fsize = get_fsize(file);

	printf("[client] sending file...(%s)\n", FILE_PATH); 
	while(nsize!=fsize){
		int fpsize = fread(send_buff, 1, 1024, file);
		nsize += fpsize;
		printf("[client] file size %dB | send to %dB\n", fsize, nsize);
		send(sock, send_buff, fpsize, 0);
	}
	
	fclose(file);
    */
    file = fopen(FILE_NAME, "wb");
	if(file == NULL){
		perror("[Client] fopen() ");
		return -1;
	}

	do{
		nsize = recv(sock, buffer, 1024, 0);
		fwrite(buffer, sizeof(char), nsize, file);
	}while(nsize!=0);
	printf("[Client] received file\n");
	
	fclose(file);
	
	close(sock);

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
