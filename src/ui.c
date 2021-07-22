#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include <curses.h>
#include "ui.h"
#include "modules.h"

WINDOW *uiwindow = NULL; //Get a pointer to the screen

int color_map[MAX_INTENSITY+1]={1, 2, 2, 3, 3, 3, 4, 4, 4, 4, 4, 5, 3, 6};

void set_colors()
{
    for(int i=0; i<8;i++)
    {
        init_pair(i+1,i,COLOR_BLACK);
    }
    //set shades of Green
    for(int i=0; i<=5;i++)
    {
        init_color(i,0,i*200,0);
    }
    init_color(6, 800,1000,800);
    return;
}

bool init_ui(void)
{
    uiwindow = initscr();
    if (uiwindow == NULL) return false;

    start_color();
    if (!has_colors() || !can_change_color() || COLOR_PAIRS < 6)
    {
        perror("[!] " RED("Your Terminal Cannot Run This Program!\n"));
        return false;
    }

    set_colors();
    return true;
}

void cleanup_ui(void)
{
    delwin(uiwindow); //Delete Window
    endwin();         
    refresh();        //Refresh Screen
}

void show_matrix(void)
{
    //set the matrix to all black
    for (int X=0; X < MAX_X; X++)
    {
        for (int Y=0; Y < MAX_Y; Y++)
        {
            int intensity=matrix[X][Y].intensity;
            color_set(color_map[intensity], NULL);
            mvaddch(Y, X, matrix[X][Y].char_value);
        }
    }
   refresh();
}

