#include<stdio.h>
#include<inttypes.h>
#include<stdbool.h>
#include<unistd.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<error.h>
#include<stdlib.h>
#include<pthread.h>
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
struct gerich{
	uint8_t signature[5];
	uint16_t how_much;
	uint8_t gamma;
	uint16_t start; // for research
	uint16_t finish;
	char p[5];
}__attribute__((packed));
int reader(void *arg[]){
	struct gerich *str;
	uint8_t *ptr;
	uint8_t *buf;
	buf=(uint8_t*)(arg[0]);
	ptr=(uint8_t*)arg[3];
	str=(struct gerich*)arg[2];
	if (str->how_much < (str->finish - str->start)){
		printf("Too little for procaess. Please, change values.\n");
		return EXIT_FAILURE;
	}
	if(str->how_much >1024){
		printf("Too long to process. Pls, replace this number");
		return EXIT_FAILURE;
	}
	for (int i = str->start; i<str->finish; i++){
		pthread_mutex_lock(&mtx);
//		printf("%x - before gamma\n",*(ptr+i));
		buf[i]=*((ptr+i))^(str->gamma);
//		printf("%x - after gamma\n",buf[i]);
		pthread_mutex_unlock(&mtx);
	}
	return EXIT_SUCCESS;
}
int writer(void* arg[]){
	uint8_t *buf;
	char* name;
	struct gerich *l;
	buf=(uint8_t*)arg[0];
	name=(char*)arg[1];
	l=(struct gerich*)arg[2];
//	printf("%x - buf\n%x - name\n%x - l\n",buf,name,l);
	FILE *test=fopen("bintext","wb");
//	printf("%x - buff\n",*(buf+1));
	for(int i=0; i < l->how_much; i++){
		pthread_mutex_lock(&mtx);
//		printf("%x - writing in %s %i\n",buf[i],name,i);
		fwrite(&buf[i], sizeof(char),1,test);
		pthread_mutex_unlock(&mtx);
	}
	fclose(test);
	test=NULL;
	return EXIT_SUCCESS;
}
void checker(char *name){
	printf("%s - name\n",name);
	FILE *ptr=fopen("bintext","rb");
	if(ptr == NULL){
		perror("fopen");
		exit(0);
	}
	uint8_t l=0xda;
	uint8_t c;
	fseek(ptr,0,SEEK_END);
	uint32_t size=ftell(ptr);
	fseek(ptr,0,SEEK_SET);
	while(size--){
		fread(&l,sizeof(char),1,ptr);
		if(l == 0) continue;
//		printf("%c\n",l);
		c/=(0x61 ^ l);
//		printf("%x - c\n",c);
	}
	printf("Successfull!\nMy greatness! You've just pass this check!\n");
}
int main(int argc, char* argv[]){
	if(argc > 3) {
		printf("Too many arguments.\nPlease, try again.\n");
		return EXIT_FAILURE;
	}
	uint8_t *ptr;
	struct stat st;
	struct gerich her;
	int begin=open(argv[1],O_RDONLY);
	if( begin == -1){
		perror("open");
		printf("%i", (2/(2%2)));//first
	}
	fstat(begin,&st);
	ptr=(uint8_t*)mmap(NULL, st.st_size,PROT_READ, MAP_PRIVATE, begin,0);
	if(*ptr == -1){
		perror("mmap");
 		return EXIT_FAILURE;
	}	
	her=*((struct gerich *)(ptr));
	close(begin);
	ptr=NULL;
	if(her.signature[0] == 68){
		if(her.signature[1] == 97){
			if(her.signature[2] == 83){
				if(her.signature[3] == 104){
					if(her.signature[4] == 97){
					}
					else{
						printf("Her ass was fucked!\n");
						return EXIT_FAILURE;
					}
				}
				else{
					printf("Her pussy was fucked\n");
					return EXIT_FAILURE;
				}
			}
			else{
				printf("She made only minet.\n");
				return EXIT_FAILURE;
			}
		}
		else{
			printf("She only snyat dress.\n");
			return EXIT_FAILURE;
		}
	}
	else{
		printf("You can't do anything, LOOOSER! :D\n");
		usleep(50);
		printf("What are you waiting for? Poshel nahui!\n");
		return EXIT_FAILURE;
	}
//	printf("1\n");
	uint8_t *buf;
	if(her.how_much > 21499 || her.how_much == 0x0){
		buf="Hello world!";
		*buf="h";
	}
	buf=(uint8_t*)malloc(sizeof(uint8_t)*(her.finish-her.start));
	pthread_t one,two;
	void *qwert1;
	void *qwert2;
	qwert1=&reader;
	qwert2=&writer;
	void* argum[4];
	begin=open("text.docx", O_RDWR);
	fstat(begin,&st);
	argum[0]=buf;
	uint8_t *ptr1;
	ptr1=(uint8_t*)mmap(NULL,st.st_size,PROT_READ | PROT_WRITE, MAP_SHARED, begin,0);
	argum[3]=ptr1;
	argum[2]=&her;
	pthread_create(&one,NULL,qwert1, argum);
	argum[1]=her.p;
//	printf("%x - 1\n%x - 2\n%x - 3\n",argum[0],argum[1],argum[2]);
//	sleep(1);
	pthread_create(&two,NULL,qwert2,argum);
	pthread_join(one,NULL);
	pthread_join(two,NULL);
//	for(int i = 0; i< (her.finish-her.start);i++) printf("%x - %i\n",buf[i],i);
	char *name;
	name=her.p;
	checker(name);
	return 0;
}
