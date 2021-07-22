#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <math.h>
#include <unistd.h>

// Colors
#define RED(string)     "\x1b[31m" string "\x1b[0m"
#define GREEN(string)   "\x1b[32m" string "\x1b[0m"
#define YELLOW(string)  "\x1b[33m" string "\x1b[0m"
#define BLUE(string)    "\x1b[34m" string "\x1b[0m"
#define MAGENTA(string) "\x1b[35m" string "\x1b[0m"
#define CYAN(string)    "\x1b[36m" string "\x1b[0m"

static inline double rand01()
{
    return (double)rand() /(double)RAND_MAX;
}

//Read characters from a file
static inline char *READ_FILE(const char *filepath)
{
    FILE *f=fopen(filepath,"rb");
    if (f == NULL )
    {
        perror("[+] " RED("File Couldn't Be Opened For Reading"));
        exit(EXIT_FAILURE);
    }
    fseek(f,0,SEEK_END);
    long fsize = ftell(f);
    rewind(f);  /* same as rewind(f); */

    char *string = (char *)malloc(fsize + 1);
    fread(string, 1, fsize, f);
    fclose(f);
    string[fsize] = 0;

    return string;
}

// Get Screen Size
static inline int GET_DIMENSIONS(int choice)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    if (choice==0)
    {
        return (int)w.ws_col; //Return Number Of colums (X)
    }
    else if (choice==1)
    {
        return (int)w.ws_row; //Return Number Of Rows (Y)
    }   
    else {
        perror("[-]" RED("Function Called With Invalid Arguments"));
        exit(EXIT_FAILURE);
    } 
}

