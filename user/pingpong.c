#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"



int
main(int argc, char *argv[])
{
    int parent_pipe[2];
    int child_pipe[2];    
    pipe(parent_pipe);
    pipe(child_pipe);

    char* buffer=(char *)malloc(sizeof(char));
    char msg='0';

    if(fork() == 0) {
        
        if(read(child_pipe[0],buffer, 1)){
            int chl_pid=getpid();
            printf("%d: received ping\n",chl_pid);            
        }
        
        write(parent_pipe[1], &msg, 1);
                
    } 
    else {
   
        write(child_pipe[1], &msg, 1);

        if(read(parent_pipe[0],buffer, 1)){
            int par_pid=getpid();
            printf("%d: received pong\n",par_pid);            
        }
     
    }

    free(buffer);
    exit(0);
}
