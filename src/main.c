//Written by @whokilleddb
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <curses.h>
#include <stdbool.h>
#include "ui.h"
#include "matrix.h"

#define ITERATIONS 500
#define REFRESH_DELAY 50000L


int main(int argc,char *argv[])
{
    //Initialize Curses
    if(!init_ui())
    {
        return EXIT_FAILURE;
    }

    //Set Up Basic Data Model
    matrix_init(); 

    for(int i=0; i < ITERATIONS; i++)
    {
        matrix_update();
        show_matrix();
        usleep(REFRESH_DELAY);
    }

    cleanup_ui();
    return EXIT_SUCCESS;
}
