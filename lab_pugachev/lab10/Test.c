#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <malloc.h>
#include<fcntl.h>

int client(uint16_t port, char* src, char* dst, char* from, char* dist)
{
	int sd,ret, scan;
	int *size;
	uint32_t *rea;
	struct sockaddr_in addr;
	int *list;
	char buf[512];
	struct dirent **ent;
	list=(int*)malloc(30);
	
	if ( (-1) == (sd = socket(AF_INET, SOCK_STREAM, 0)) ) {
		perror("socket");
		return __LINE__;
	}
	memset(&addr,0,sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = port;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	//sleep(3);
	
	if ( connect(sd, (const struct sockaddr*)&addr,
				 sizeof(struct sockaddr_in)) ) {
		perror("connect");
		close(sd);
		return __LINE__;
	}
	for(;;){

		snprintf(buf,sizeof(buf), "./%s",from);
		scan=scandir(buf, &ent, NULL, alphasort);
		while(scan--){
			if(ent[scan]->d_type!=0x4)
			{
				snprintf(buf,sizeof(buf), "./%s/%s", from, ent[scan]->d_name);
				//printf("%s\n", buf);
				FILE *f=fopen(from, "rb");
				if(f == NULL) {
					printf("trouble!");
					return 0;
				}
				fseek(f,0, SEEK_END);
				size=ftell(f);
				fseek(f,0,SEEK_SET);
				printf("%i\n", size);
				send(sd, ent[scan]->d_name,sizeof(ent[scan]->d_name), 0);
				send(sd,&size, sizeof(int),0);
				while ( (ret = fread(&rea, 1, sizeof(int), f)) > 0 ) {
					send(sd, &rea, ret, 0);
				}
				fclose(f);
				close(sd);
			}
		}
	}
	return 0;
}

int server(uint16_t port, char* src, char* dst, char* from, char* dist)
{
	int sd, clsd,rea, ret;
	struct sockaddr_in addr;
	struct dirent *here;
	struct stat st;
	char buf[512];
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
	snprintf(buf, sizeof(buf), "./%s",dist);
	for(;;){
		printf("1\n");
		recv(clsd,here->d_name,sizeof(here->d_name),0);
		recv(clsd,&(st.st_size),sizeof(st.st_size),0);
		for(int i=0; i<st.st_size; i++){
			snprintf(buf,sizeof(buf),"./%s%s", dist, here->d_name);
			FILE *w=fopen(buf, "wb");
			if(w == NULL) printf("trouble1!");
			while((ret = recv(clsd,&rea,sizeof(int),0)) > 0) {
				fwrite(&rea, 1, ret,w);
			}
			fclose(w);
		}
	}
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
