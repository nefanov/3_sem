#include <signal.h>
#include <stdio.h>
#include <errno.h>

int main() {
  sigset_t set;
  int sig;
  int *sigptr = &sig;
  int ret_val;
  sigemptyset(&set);
  sigaddset(&set, SIGINT);
  sigprocmask( SIG_BLOCK, &set, NULL );

  printf("Waiting for a SIGINT signal\n");

  ret_val = sigwait(&set,sigptr);
  if(ret_val == -1)
     perror("sigwait failed\n");
  else {
     if(*sigptr == 2)
        printf("SIGINT was received\n");
     else
        printf("sigwait returned with sig: %d\n", *sigptr);
  }
  return 0;
}
