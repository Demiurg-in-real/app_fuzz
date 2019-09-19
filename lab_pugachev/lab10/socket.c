#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int client(uint16_t port, char* src, char* dst, char* from, char* dist)
{
	int sd,ret;
	uint32_t *rea;
	struct sockaddr_in addr;
	
	if ( (-1) == (sd = socket(AF_INET, SOCK_STREAM, 0)) ) {
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
	FILE *f=fopen(from, "rb");
	if(f == NULL) {
		printf("trouble!");
		return 0;
	}
	
	while ( (ret = fread(&rea, 1, sizeof(int), f)) > 0 ) {
		send(sd, &rea, ret, 0);
	}
	/*do {
		fread(&rea, sizeof(int),1,f);
		send(sd,&rea, sizeof(int), 0);
	}
	while(rea != EOF);*/
	fclose(f);
	//uint32_t *end;
	//*end=0xfabecd12;
	//write(sd, end,14);
	close(sd);
	return 0;
}

int server(uint16_t port, char* src, char* dst, char* from, char* dist)
{
	int sd, clsd,rea, ret;
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
	FILE *w=fopen(dist, "wb");
	if(w == NULL) printf("trouble1!");
/*	do {
		recv(clsd,&sd,sizeof(int),0);
		fwrite(&sd, sizeof(int),1,w);
	} while(sd != 0xfabecd12);
*/
	while((ret = recv(clsd,&rea,sizeof(int),0)) > 0) {
		fwrite(&rea, 1, ret,w);
	}
	fclose(w);
	close(clsd);
	close(sd);
	return 0;
}


int main(int argc, char* argv[])
{
	pid_t pid;
	uint16_t port;

	if ( argc != 5 ) {
		fprintf(stderr,"Usage: %s <src> <dst>\n",argv[0]);
		return 0;
	}
	
	port = getpid();
	if (port <= 1000 ) port += 1000;
	if ( (-1) == (pid = fork()) ) {
		perror("fork");
		return __LINE__;
	}
	
	return ((pid != 0)?(server):(client))(port,argv[1],argv[2], argv[3],argv[4]);
}
