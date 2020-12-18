#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int size = 2000000;

char** parse_cmd(const char* cmd)
{
        int n = strlen(cmd), a = 0;


        for (int i = 1; i < n; i++)
                if ((cmd[i] != ';' && cmd[i] != ' ' && cmd[i] != '_') && (i == 0 || cmd[i-1] == ';' || cmd[i-1] != ' ' || cmd[i-1] != '_'))
                        a++;
        char** q;
        q = (char**)malloc((a + 1) * sizeof(char*));
        int i = 0;
        int number = 0;
        while(i < n)
        {
                if (cmd[i] == ';' || cmd[i] == ' ' || cmd[i] == '_')
                    i++;
                else
                {
                        int j = i;
                        while(j < n && cmd[j] != ';' && cmd[j] != ' ' && cmd[j] != '_')
                            j++;

                        char* c = (char *)malloc((j - i) * sizeof(char));
                        int cc = 0;
                        while(i < j)
                        {
                                c[cc] = cmd[i];
                                cc++;
                                i++;
                        }
                        q[number] = c;
                        number++;
                }
        }
	q[number] = NULL;
        return q;
}


static void run_cmd(const char *cmd)
{
	const pid_t pid = fork();
  	if (pid < 0) 
	{
  	     printf("fork failed!\n");
  	 	 return;
 	 }
 	 if (pid) 
	 {
		 int status;
   		 waitpid(pid, &status, 0);
   		 printf("\nRet code: %d\n", WEXITSTATUS(status));
   		 return;	
	 }
	 char **args = parse_cmd(cmd);
	 execvp(args[0], args);
 	
	 printf("exec* failed\n");
}

int main()
{
	while(1) 
	{
	    char* cmd;
		cmd = (char*)malloc(size * sizeof(char));
		gets(cmd);
		run_cmd(cmd);
		exit(12);
	}
	return 0;
}
