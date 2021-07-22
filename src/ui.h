#ifndef _UI_H_
#define _UI_H_

#include <stdbool.h>

#define MAX_X 160
#define MAX_Y 50

#define MAX_INTENSITY 13
#define MIN_INTENSITY 2

#define BRIGHTNESS 6

// Contains each Cell's character and Intensity
typedef struct
{
    char char_value;
    int intensity;
} cell;

extern cell matrix[MAX_X][MAX_Y];

//prototypes
bool init_ui();
void cleanup_ui();
void show_matrix();
#endif