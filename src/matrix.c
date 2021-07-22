#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "ui.h"
#include "modules.h"

#define PROB_DRIP_SPAWN 0.65
#define PROB_DIM 0.55
#define PROB_CHANGE 0.95
#define RANDOM_PRINTABLE_CHARACTER (33 + (rand()%80))
#define DRIP_NUM 2500

typedef struct {
    int x, y;
    bool live;
    bool bright;
} drip;

cell matrix[MAX_X][MAX_Y];
drip drips[DRIP_NUM];

void try_add_drips()
{
    for(int i=0; i<DRIP_NUM; i++)
    {
        if (drips[i].live == false)
        {
            drips[i].live=true;
            drips[i].x = rand() % MAX_X;
            drips[i].y =0; //rand() % MAX_Y
            drips[i].bright=rand()%2;
            return;
        }
    }
}

void update_drips()
{
    for (int i=0; i<DRIP_NUM; i++)
    {
        if(drips[i].live)
        {
            if(drips[i].bright)
            {
                matrix[drips[i].x][drips[i].y].intensity=MAX_INTENSITY;
            }
            else
            {
                matrix[drips[i].x][drips[i].y].intensity=MIN_INTENSITY;
            }
            //drips die when they leave screen
            if(++drips[i].y >=MAX_Y-1)
            {
                drips[i].live=false;
            }
        }
    }
}

void init_drips()
{
    int i;
    for(i=0;i<DRIP_NUM;i++)
    {
        drips[i].live=false;
    }
}

void matrix_init()
{
    //set the matrix to all black
    for (int X=0; X < MAX_X; X++)
    {
        for (int Y=0; Y < MAX_Y; Y++)
        {  
            matrix[X][Y].char_value=0;
            matrix[X][Y].intensity=0;
        }
    }
    init_drips();
}

void fade_n_change_matrix(void)
{
    for (int X=0; X < MAX_X; X++)
    {
        for (int Y=0; Y < MAX_Y; Y++)
        {
            //Randomly change characters
            if (rand01() < PROB_CHANGE || matrix[X][Y].char_value == 0)
            {
                matrix[X][Y].char_value = RANDOM_PRINTABLE_CHARACTER;
            }
            //Randomly dim the cells
            if(rand01() < PROB_DIM)
            {
                if (matrix[X][Y].intensity > 0)
                {
                    matrix[X][Y].intensity--;
                }    
            }            
        }
    }    
}

void matrix_update()
{
    if (rand01() < PROB_DRIP_SPAWN)
    {
        try_add_drips();
    }
    update_drips();
    fade_n_change_matrix();
}


