#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"



int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    fprintf(2, "Usage: sleep missing parameter...\n");
    exit(1);
  }

  
  i=atoi(argv[1]);


  if(i <= 0){
    fprintf(2, "sleep: tick number should be positive\n");
    exit(1);
  }

  sleep(i);
  exit(0);
}