/*
//  main.c
//  perror example
//
//  Created by Ariel Feinerman on 23/03/17.
//  Copyright  2017 Feinerman Research, Inc. All rights reserved.
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, const char * argv[]) 
{
    // Generate unique filename.
    char *file_name = tmpnam((char[L_tmpnam]){0});
   
    errno = 0;
    FILE *file = fopen(file_name, "rb");

    if (file) {
        // Do something useful. 
        fclose(file);
    }
    else {
        perror("fopen() ");
    }
	
    return EXIT_SUCCESS;
}
