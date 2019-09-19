#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
//Очень важный код, разберись бл!
struct list_head
{
	struct list_head *next, *prev;
};

struct product_s {
	int val;
	struct list_head  list;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)


static inline void list_add(struct list_head *item, struct list_head *head)
{
	struct list_head *next = head->next;
	struct list_head *prev = head;

	next->prev = item;
	item->next = next;
	item->prev = prev;
	prev->next = item;
}

static inline int list_empty(const struct list_head *head)
{
	return head->next == head;
}

static inline void list_del(struct list_head *entry)
{
	struct list_head *prev, *next;

	prev = entry->prev;
	next = entry->next;

	next->prev = prev;
	prev->next = next;

}

#define offsetof(TYPE, MEMBER)			\
    ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member)	({				\
	    const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	    (type *)( (char *)__mptr - offsetof(type,member) );})

#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)


//Здесь кончается ГНУсный код
LIST_HEAD(stack);
int work_flag = 1;

void to_stop(int num)
{
	work_flag = 0;
}


int main(int argc, char *argv[])
{
	int i;
	struct product_s *item;
	struct list_head *ptr;
	useconds_t timeout;

	
	srand(time(NULL) + getpid());
	signal(SIGINT,to_stop);
	int constant=4;
	int lang, ch;
	char *tikai;
	FILE *textolit=fopen("1.txt", "a+");
	FILE *corpa=fopen("2.txt", "w");
	if (textolit==NULL && corpa==NULL)
	{
		printf ("slomalos");
	}



	while( work_flag ) {
		for ( i = 0 ; i < 10 ; i++) {
			if ( NULL == (item = malloc(sizeof(struct product_s)))) {
				perror("malloc");
				return __LINE__;
			}
			item->val = rand() & 0xffff;
			list_add(&(item->list),&stack);
			fwrite("%04X\n",item->val,1,textolit);
			printf("%04X\n",item->val);
		}

		timeout = rand();
		timeout = (timeout % 6 ) + 5;
		usleep(timeout);
	}
	fclose(textolit);
	fclose(corpa);
    return 0;
}

void ORiat()
	{	
	FILE *textolit=fopen("1.txt", "a+");
	FILE *corpa=fopen("2.txt", "w");
	if (textolit==NULL && corpa==NULL)
	{
		printf ("slomalos");
	}


		while ( !list_empty(&stack) ) {
			ptr = stack.next;
			list_del(ptr);
			item = list_entry(ptr, struct product_s, list);
			int ch;
			while ((ch= fgetc(textolit))!=EOF)
			{
				fputs(ch,corpa);
			}
			fprintf(stderr,"%04X\n",item->val);
			free(item);
		}
	}
void IFIAT ()
{
FILE *textolit=fopen("1.txt", "a+");
if (textolit==NULL)
	{
		printf ("slomalos");
	}

for ( i = 0 ; i < 10 ; i++) {
			if ( NULL == (item = malloc(sizeof(struct product_s)))) {
				perror("malloc");
				return __LINE__;
			}
			item->val = rand() & 0xffff;
			list_add(&(item->list),&stack);
			fwrite("%04X\n",item->val,1,textolit);
			printf("%04X\n",item->val);
		}

		timeout = rand();
		timeout = (timeout % 6 ) + 5;
		usleep(timeout);


}

