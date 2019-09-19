#define _DEFAULT_SOURCE
       #include <dirent.h>
       #include <stdio.h>
       #include <stdlib.h>

       int
       main(void)
       {
           struct dirent **namelist;
           int n;

           if(scandir("het", &namelist, NULL, alphasort) == -1){
           //if (n == -1) {
               printf("Hey!\n");
		   perror("scandir");
               exit(EXIT_FAILURE);
           }

           while (n--) {
               printf("%s\n", namelist[n]->d_name);
               free(namelist[n]);
           }
           free(namelist);

           exit(EXIT_SUCCESS);
       }
