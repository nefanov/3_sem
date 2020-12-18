#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <stdlib.h>

void CopyDirectory(const char *bname, const char * lname) 
{
	DIR *backup_dir = opendir(bname);
	DIR *looking_dir = opendir(lname);	
	if(looking_dir)
	{
       		char Path[PATH_MAX], *EndPtr = Path;
        	struct stat info;
		char backup_path[PATH_MAX];
        	struct dirent *e;
        	strcpy(Path, lname);
        	EndPtr += strlen(lname);          
        	while ((e = readdir(looking_dir)) != NULL) 
		{ 
			strcpy(backup_path, bname);
            		strcpy(EndPtr, e -> d_name); 
            		if (!stat(Path, &info)) 
			{   
				if (S_ISDIR(info.st_mode)) 
				{
					if (Path[strlen(Path) - 1] != '.')
					{
						CopyDirectory(bname, strcat(Path, "/"));
					}
                		}
			       	else if (S_ISREG(info.st_mode)) 
				{
                    			int fd = open(strcat(backup_path, EndPtr), O_RDWR | O_CREAT, 0777);
					const pid_t pid = fork();
					if (pid == 0)
					{
						execlp("cp", "cp", Path, backup_path, NULL);
					}
                		}
            		}
        	}
    	}
    	return;
}


void CheckDirectory(const char *bname, const char * lname)
{
        DIR *backup_dir = opendir(bname);
        DIR *looking_dir = opendir(lname);
        if(looking_dir)
        {
                char Path[PATH_MAX], *EndPtr = Path;
                struct stat info;
                char backup_path[PATH_MAX];
                struct dirent *e;
                strcpy(Path, lname);
                EndPtr += strlen(lname);
                while ((e = readdir(looking_dir)) != NULL)
                {
                        strcpy(backup_path, bname);
                        strcpy(EndPtr, e -> d_name);
                        if (!stat(Path, &info))
                        {
                                if (S_ISDIR(info.st_mode))
                                {
                                        if (Path[strlen(Path) - 1] != '.')
                                        {
                                                CheckDirectory(bname, strcat(Path, "/"));
                                        }
                                }
                                else if (S_ISREG(info.st_mode))
                                {
                                        int fd = open(strcat(backup_path, EndPtr), O_RDWR | O_CREAT, 0777);
					char diff[PATH_MAX] = "diff ";
					strcat(diff, backup_path);
					strcat(diff, " ");
					strcat(diff, Path);
					int res = system(diff);
					if (res != 0)
					{
						const pid_t pid = fork();
						if (pid == 0)
						{
							execlp("cp", "cp", Path, backup_path, NULL);
						}
					}
                                }
                        }
                }
        }
}


int main()
{
	printf("%d\n", getpid());
	daemon(1, 1);
	int res = mkdir("backup_dir", 0777);
	printf("%d\n", res);
	char backup_dir[PATH_MAX] = "/home/fastem/3_sem/task_6/backup_dir/";
	char looking_dir[PATH_MAX] = "/home/fastem/3_sem/task_6/testdir/";
	CopyDirectory(backup_dir, looking_dir);
	while(1)
	{
		sleep(5);
		CheckDirectory(backup_dir, looking_dir);
	}
}
