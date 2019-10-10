#include<pthread.h>
//#include<iostream>
#include<unistd.h>
#include<stdio.h>
int glob=0;
static pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER;
struct tuto{
	unsigned int ger[2];
	unsigned char kuku;
	unsigned char alph[256];
}__attribute__((packed));

void *prin(void *args){
//	std::cout<<"Entered\n";
	struct tuto *mass = (struct tuto *)args;
//	std::cout<<mass[0];
	for (int i=mass->ger[0];i<mass->ger[1];i++) {
		printf("%x\n",mass->kuku);
		mass->kuku++;
		pthread_mutex_lock(&mtx);
		mass->ger[glob]=mass->kuku;
		glob++;
		pthread_mutex_unlock(&mtx);
	}
	return 0;
}
int main(){
	pthread_t pth[2];
	struct tuto *he;
	printf("%08x\n",he);
	for (int j=0;j<2;j++){
		printf("2\n");
		he->ger[0]=(j*256/2);
		printf("1\n");
		he->ger[1]=(j+1)*256/2;
		he->kuku=(unsigned char)(256/2*j);
		printf("%i -0; %i -1\n",he->ger[0],he->ger[1]);
		pthread_create(&pth[j],NULL,prin,(void *)he);
	}
	sleep(1);
/*	int i=0;
	char swap_cnt=0;
	unsigned char swap;
	while( i<256){
		if( (i+1) != 256 && he->alph[i] > he->alph[i+1]){
			swap = he->alph[i];
			he->alph[i] = he->alph[i+1];
			he->alph[i+1] = swap;
			swap_cnt=1;
		}
		i++;
		if(i == 256 && swap_cnt == 1){
			swap_cnt=0;
			i=0;
		}
	}
	for (i=0;i<256;i++) printf("%i\n", he->alph[i]);
*/	printf("%i - real\n",glob);
	return 0;
}
