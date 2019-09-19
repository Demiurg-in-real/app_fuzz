#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include<stdint.h>

int client(uint16_t port, char* src, char* dst)
{
	int sd;
	struct sockaddr_in addr;
	uint8_t buf[40]={0x90,0x00,0x00, 0x16, 0x81,0x27,0x89,0xb0, 0x00, 0x00,0x00,0x00,0xa0,0x02,0xaa,0xaa,0xfe,0x30,0x00,0x00,0x02,0x04,0xff, 0xd7,0x04,0x02,0x08,0x0a,0xf5,0x15,0xce,0x1c,0x00,0x00,0x00,0x00,0x01,0x03,0x03,0x07};
	if ( (-1) == (sd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) ) {
		perror("socket");
		return __LINE__;
	}
	memset(&addr,0,sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = port;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	sleep(3);
	
	if ( connect(sd, (const struct sockaddr*)&addr,
				 sizeof(struct sockaddr_in)) ) {
		perror("connect");
		close(sd);
		return __LINE__;
	}

	write(sd,buf, 40);
	close(sd);
	return 0;
}

int server(uint16_t port, char* src, char* dst)
{
	int sd, clsd;
	struct sockaddr_in addr;
	socklen_t addr_size;
	char msg[13]="ERROR!!!!!!!\n";
	if ( (-1) == (sd = socket(AF_INET,SOCK_STREAM, 0)) ) {
		perror("socket");
		return __LINE__;
	}
	memset(&addr,0,sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = port;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if ( bind(sd, (const struct sockaddr*)&addr,
				 sizeof(struct sockaddr_in)) ) {
		perror("bind");
		close(sd);
		return __LINE__;
	}

	if ( (-1) == listen(sd, 1) ) {
		perror("listen");
		close(sd);
		return __LINE__;
	}
	addr_size = sizeof(struct sockaddr_in);
	if ( (-1) == (clsd = accept(sd,
								(struct sockaddr*) &addr,
								&addr_size))) {
		perror("accept");
		close(sd);
		return __LINE__;
	}
	read(clsd, msg, 13);
	write(1,msg,13);
	close(clsd);
	close(sd);
	return 0;
}


int main(int argc, char* argv[])
{
	pid_t pid;
	uint16_t port;

	if ( argc != 3 ) {
		fprintf(stderr,"Usage: %s <src> <dst>\n",argv[0]);
		return 0;
	}
	
	port = getpid();
	if (port <= 1000 ) port += 1000;
	if ( (-1) == (pid = fork()) ) {
		perror("fork");
		return __LINE__;
	}
	
	return ((pid != 0)?(server):(client))(port,argv[1],argv[2]);
}
