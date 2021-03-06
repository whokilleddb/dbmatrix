//Written by @whokilleddb
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <curses.h>
#include <stdbool.h>
#include <signal.h>
#include "dbmatrix.h"

int main(int argc,char *argv[])
{
    signal(SIGINT, exit_gracefully); //capture SIGNINT
    setup_global(); //setup global variables
    show_info(); //show globals

    //Initialize Curses
    if(!init_ui())
    {
        return EXIT_FAILURE;
    }

    //Set Up Basic Data Model
    matrix_init(); 
    if (ITERATIONS<=0)
    {
        while(true)
        {
            play_animation();
        }
    }
    else 
    {
        for(int i=0; i < ITERATIONS; i++)
        {
            play_animation();
        }
    }
    freepointer();
    cleanup_ui();
    return EXIT_SUCCESS;
}
