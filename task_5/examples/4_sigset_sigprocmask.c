//Программа, демонстрирующая управление группой сигналов 
//Источник: http://fkn.ktu10.com/?q=node/665
/*
Запустите программу в терминале, после чего завершите его работу
Убедитесь, что программа продолжает работать (SIGHUP блокирован):
ps aux | grep <PID>
Завершите программу, отправив сигнал SIGTERM:
kill -SIGTERM <PID>
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void term_handler(int i) // функция-обработчик сигнала
{ 
  printf ("Terminating\n");
  exit(EXIT_SUCCESS);
}

int main(int argc, char ** argv) 
{
    printf("My pid is %d\n", getpid());
    struct sigaction sa; /* спец. структура используемая в качестве параметра системного вызова sigaction() */
    sigset_t newset; // набор сигналов
    sigemptyset(&newset); // инициализирует набор сигналов, указанный в newset, и "очищает" его от всех сигналов
    sigaddset(&newset, SIGHUP); // добавляем сигнал SIGHUP в наш набор  
    sigprocmask(SIG_BLOCK, &newset, 0); // добавляем набор newset в группу заблокированных 
    sa.sa_handler = term_handler; // добавляем в структуру типа  sigaction указатель на функцию обработчик сигнала
    
    sigaction(SIGTERM, &sa, 0); /* этим вызовом с учётом всех предыдущих действий 
                    мы назначили нашему процессу обработчик term_handler на случай получения сигнала SIGTERM
                    - т.е. сигнала "предложения завершиться"*/
     
    printf("My pid is %i\n", getpid());
    printf("Waiting...\n");
     
    while(1) 
      sleep(1);
      //experiments:
     //1)add sigdelset <signal> there to del signal entry
     //2)sigprocmask(SIG_BLOCK/SIG_DFL...) on some <signal>
  
    return EXIT_FAILURE;
}
