#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{

  if(argc < 2){
    fprintf(2, "Usage: sleep for some time , you need to input time\n");
    exit(1);
  }


  int sleep_time = (int)*argv[3];

  sleep(sleep_time);

  fprintf(1,"(nothing happens for a little while)\n");

  exit(0);
}