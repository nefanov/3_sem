#include <stdio.h>
#include <signal.h>
#include <unistd.h>

// to send signal for self: kill(getpid(), SIGINT);

void sig_handler(int signo)
{
  if (signo == SIGINT)
    printf("Received: SIGINT\n");
}

int main(void)
{
  if (signal(SIGINT, sig_handler) == SIG_ERR)
    printf("\ncan't catch SIGINT\n");
  while(1) 
    sleep(1);
  return 0;
}
