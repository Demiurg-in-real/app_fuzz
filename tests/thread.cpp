#include<pthread.h>
#include<iostream>
#include<unistd.h>
int glob=0;
static pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER;
struct tuto{
	unsigned int ger[2];
	unsigned char kuku;
	unsigned char alph[256];
};//__attribute__((packed));

void *prin(void *args){
	std::cout<<"Entered\n";
	tuto *mass = (tuto *)args;
	int start=mass->ger[0];
	int finish=mass->ger[1];
	std::cout<<start<< " - " << finish<<"\n";
	for (int i=start;i<finish;i++) {
		printf("%i\n",i);//mass->kuku);
		pthread_mutex_lock(&mtx);
		mass->kuku=char(i);
		mass->alph[glob]=mass->kuku;
		printf("why - %x - why\n",mass->alph[glob]);
		glob++;
		pthread_mutex_unlock(&mtx);
	}
	return 0;
}
int main(){
	pthread_t pth[2];
	tuto *he;
	printf("%08x\n",he);
	for (int j=0;j<2;j++){
//		std::cout<<"2\n";
		he->ger[0]=(j*256/2);
		std::cout<<"1\n";
		he->ger[1]=(j+1)*256/2;
		he->kuku=(unsigned char)(256/2*j);
		printf("%i -0; %i -1\n",he->ger[0],he->ger[1]);
		pthread_create(&pth[j],NULL,prin,(void *)he);
		usleep(5);
	}
	sleep(1);
	int nin=0;
/*	char swap_cnt=0;
	unsigned char swap;
	while( nin<256){
		if( (nin+1) != 256 && he->alph[nin] > he->alph[nin+1]){
			swap = he->alph[nin];
			he->alph[nin] = he->alph[nin+1];
			he->alph[nin+1] = swap;
			swap_cnt=1;
		}
		nin++;
		if(nin == 256 && swap_cnt == 1){
			swap_cnt=0;
			nin=0;
		}
	}
*/	for (nin=0;nin<256;nin++) printf("%x - bl\n", he->alph[nin]);
//	printf("%x\n",he->alph[3]);
	printf("%i - real\n",glob);
	return 0;
}
