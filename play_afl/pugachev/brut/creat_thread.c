/* use: gcc file.c -lpthread */
#include <pthread.h>
#include <stdio.h>

static void *thread_func(void *none)
{
     printf("Message from thread\n");
     return NULL;
}

int main(int argc,char* argv[])
{
     pthread_t pid;
     void *retval;
     pthread_create(&pid,NULL,thread_func,NULL);
     printf("Message from main thread\n");
     pthread_join(pid, &retval);
     return 0;
}
