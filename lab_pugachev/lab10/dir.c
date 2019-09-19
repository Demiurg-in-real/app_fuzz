#include<stdio.h>
#include<stdint.h>
#include<dirent.h>
#include<time.h>
#include<stdlib.h>
#include<sys/stat.h>
#include <fcntl.h>
#include<unistd.h>
#include<string.h>
int main()
{
	char buf[512]; char buf1[512], buf3[512], buf4[512]; //buf1 - dir, buf2 - files in dir, buf3 - newdir
	char buf6[512],buf7[512];//bufs for check
	int n;
	char *rea;
	char check;
	char *point="..";
	char *point1=".";
	struct dirent **ent;
	struct stat st;
	unsigned char timer;
	srand(time(NULL));
	snprintf(buf,sizeof(buf), "./");
	//n=scandir(buf, &ent, NULL, alphasort);
	if (n == -1){
		perror("scandir");
		exit(EXIT_FAILURE);
	}
	do{
		n=scandir(buf,&ent,NULL,alphasort);
		while (n--){
			//snprintf(buf6,sizeof(buf6), "/.");
			//snprintf(buf7,sizeof(buf7), "/..");
			if(ent[n]->d_type == 0x4){
				if( strcmp(ent[n]->d_name,point) != 0 && strcmp(ent[n]->d_name,point1) != 0){
					printf("%s - here it is\n", ent[n]->d_name);
					snprintf(buf3,sizeof(buf3), "%s%d",buf,ent[n]->d_name);
				}
				else{
					continue;
				}
			}
			else {
				snprintf(buf1,sizeof(buf1),"%s%s", buf, ent[n]->d_name);
				timer=random();
				int ne=open(buf1,O_RDWR);
				fstat(ne,&st);
				close(ne);
				printf("%s - %d, size - %d\n", buf1, ent[n]->d_type, st.st_size);


			}
			free(ent[n]);
				/*FILE *f=fopen(buf1, "rb");
				snprintf(buf1,sizeof(buf1),"%s%i", buf1, timer);
				FILE *w=fopen(buf1, "wb");
				while ( (check=fread(&rea, sizeof(char),1,f)) != 0){
					fwrite(&rea,sizeof(char), 1,w);
				}
				fclose(f);
				fclose(w);
			}*/
		}
		printf("1\n");
		//snprintf(buf4,sizeof(buf4), "%s", buf);
		//printf("%s - %s", buf, buf3);
		//snprintf(buf,sizeof(buf), "%s", buf3);
		printf("%s || %s", buf,buf3);
	}
	while(buf3 != NULL || strcmp(buf4, buf3)==0);
	return 0;
}
				
				
