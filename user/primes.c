#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void init(){
    int i;
    for(i=2;i<36;i++){
        write(1,&i,sizeof(i));
    }
}
void redirect(int flag, int p[]){
    close(flag);   //close(0);
    dup(p[flag]);
    close(p[0]);
    close(p[1]);
}
void cull(int p){
    int n;
    while(read(0,&n,sizeof(n))){
        if(n%p!=0){
            write(1,&n,sizeof(n));
        }
    }
}
void sink(){
    int p[2];
    int num;
    
    if(read(0,&num,sizeof(num))){
        printf("prime %d\n",num);
        pipe(p);
        if(fork()==0){
            //child;
            redirect(1,p);   
            cull(num);
        }
        else{
            //parent;
            redirect(0,p);
            sink();
        }
    }
}
int
main(int argc, char* argv[]){
    int p[2];
    pipe(p);

    if(fork()==0){
        //child;
        redirect(1,p);
        init();
    }
    else{
        //parent;
        redirect(0,p);
        sink();
    }
    exit(0);
}