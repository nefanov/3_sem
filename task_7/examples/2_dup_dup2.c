#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>

int main()
{
    int file_desc = open("dup2.txt",O_WRONLY | O_APPEND);

    // here the newfd is the file descriptor of stdout (i.e. 1)
    dup2(file_desc, 1);
    // in the case od dup(...) is will be like:
    // int copy_desc = dup(1);

    // All the printf statements will be written in the file
    printf("This string will be printed in the file dup2.txt\n");

    return 0;
}
