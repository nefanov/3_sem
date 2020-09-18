#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
int str_size = 20000; //size of input

char ** parse_cmd(const char * cmd)
{
        int len = strlen(cmd);
        int counter = 0; //number of words 
        for (int i = 0; i < len; i++)
        {
                if ((cmd[i] != ' ' && cmd[i] != ';' && cmd[i] != '_') && (i == 0 || cmd[i - 1] == ' ' || cmd[i - 1] != ';' || cmd[i - 1] != '_'))
                {
                        counter++;
                }
        }
        char** parsestr; //answer
        parsestr = (char**)malloc((counter + 1) * sizeof(char*));
        int i = 0;
        int numb_word = 0; //number of current word
        while(i < len)
        {
                if (cmd[i] == ' ' || cmd[i] == ';' || cmd[i] == '_')
                {
                        i++;
                }
                else //current word in q
                {
                        int j = i;
                        while(j < len && cmd[j] != ' ' && cmd[j] != ';' && cmd[j] != '_')
                        {
                                j++;
                        }
                        char* q = (char *)malloc((j - i) * sizeof(char));
                        int qq = 0;
                        while(i < j)
                        {
                                q[qq] = cmd[i];
                                qq++;
                                i++;
                        }
                        parsestr[numb_word] = q;//current word in parsestr
                        numb_word++;
                }
        }
	parsestr[numb_word] = NULL;
        return parsestr;
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
	execv(args[0], args);
 	printf("exec* failed\n");
}

int main()
{
	while(1) 
	{
	    	char* cmd;
		cmd = (char*)malloc(str_size * sizeof(char));
		gets(cmd);
		run_cmd(cmd);
		exit(12);
	}
	return 0;
}

