#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include<sys/wait.h>



int main(int argc, char* argv[])
{
    //char* arguments[] = {"cat","tricky.txt", NULL};
    char* arguments[] = {"./son1", NULL};
    int my_pipe[2];
    if(pipe(my_pipe) == -1)
    {
       fprintf(stderr, "Error creating pipe\n");
    }

    pid_t child_id;
    child_id = fork();
    if(child_id == -1)
    {
        fprintf(stderr, "Fork error\n");
    }
    if(child_id == 0) // child process
    {
        close(my_pipe[0]); // child doesn't read
        dup2(my_pipe[1], 1); // redirect stdout

        execve(arguments[0], arguments,NULL);

        fprintf(stderr, "Exec failed\n");
    }
    else
    {
        close(my_pipe[1]); // parent doesn't write

        char reading_buf[1];
	sleep(2);

        while(read(my_pipe[0], reading_buf, 1) > 0)
        {
           write(1, reading_buf, 1); // 1 -> stdout
        }

        close(my_pipe[0]);
	int wst;
        wait(&wst);
   }
}
