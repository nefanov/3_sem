#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
int size = 2000000;

char ** parse_cmd(const char * cmd)
{
	int i;
	int len = strlen(cmd);
	int k = 0;
	for (i = 0; i < len; i++)
	{
		if ((cmd[i] != ' ' && cmd[i] != ';' && cmd[i] != '_') && (i == 0 || cmd[i - 1] == ' ' || cmd[i - 1] != ';' || cmd[i - 1] != '_'))
		{
			k++;
		}
	}
	char** args;
	args = (char**)malloc((k + 1) * sizeof(char*));
	int i1 = 0;
	int q = 0;
	while(i1 < len)
	{
		if (cmd[i1] == ' ' || cmd[i1] == ';' || cmd[i1] == '_')
        {
			i1++;
        }
        else
        {
			int j = i1;
            while(j < len && cmd[j] != ' ' && cmd[j] != ';' && cmd[j] != '_')
            {
				j++;
            }
            char* val = (char *)malloc((j - i1) * sizeof(char));
			int valq = 0;
            while(i1 < j)
            {
				val[valq] = cmd[i1];
                valq++;
                i1++;
            }
			args[q] = val;
			q++;
		}
	}
	args[q] = NULL;
	return args;
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
	char* cmd;
	int fl = 1;
	while (1)
	{
	cmd = (char*)malloc(size * sizeof(char));
	fl = 1;
	gets(cmd);
	fl = strcmp(cmd, "qu");
	if (fl==0)
		return 0;
	run_cmd(cmd);
	}	
	return 0;
}
