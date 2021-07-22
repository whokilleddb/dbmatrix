#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <curses.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>

// Colors
#define RED(string)     "\x1b[31m" string "\x1b[0m"
#define GREEN(string)   "\x1b[32m" string "\x1b[0m"
#define YELLOW(string)  "\x1b[33m" string "\x1b[0m"
#define BLUE(string)    "\x1b[34m" string "\x1b[0m"
#define MAGENTA(string) "\x1b[35m" string "\x1b[0m"
#define CYAN(string)    "\x1b[36m" string "\x1b[0m"

//Macros
#define ITERATIONS 200        //controls for how long the program will run(Infinietly if <=0)
#define REFRESH_DELAY 50000L  //controls speed 
#define MAX_INTENSITY 13      //drip length     
#define MIN_INTENSITY 5
#define BRIGHTNESS 8
#define PROB_DRIP_SPAWN 0.65
#define PROB_DIM 0.20
#define PROB_CHANGE 0.80
#define RANDOM_PRINTABLE_CHARACTER (33 + (rand()%80))
#define DRIP_NUM 2500

//Strucutres
typedef struct   // Contains each cell's character and Intensity
{
    char char_value;
    int intensity;
} cell;

//drip
typedef struct {
    int x, y;
    bool live;
    bool bright;
} drip;

//Globals
int color_map[MAX_INTENSITY+1];
WINDOW *uiwindow = NULL; //Get a pointer to the screen
int MAX_X; //No of Columns
int MAX_Y; //No of Rows
cell **matrix;
drip drips[DRIP_NUM];
int counter=0;
char rickroll[]="We're no strangers to love You know the rules and so do I A full commitment's what I'm thinking of You wouldn't get this from any other guy I just wanna tell you how I'm feeling Gotta make you understand Never gonna give you up Never gonna let you down Never gonna run around and desert you Never gonna make you cry Never gonna say goodbye Never gonna tell a lie and hurt you We've known each other for so long Your heart's been aching but you're too shy to say it Inside we both know what's been going on We know the game and we're gonna play it And if you ask me how I'm feeling Don't tell me you're too blind to see Never gonna give you up Never gonna let you down Never gonna run around and desert you Never gonna make you cry Never gonna say goodbye Never gonna tell a lie and hurt you Never gonna give you up Never gonna let you down Never gonna run around and desert you Never gonna make you cry Never gonna say goodbye Never gonna tell a lie and hurt you Never gonna give, never gonna give (Give you up) We've known each other for so long Your heart's been aching but you're too shy to say it Inside we both know what's been going on We know the game and we're gonna play it I just wanna tell you how I'm feeling Gotta make you understand Never gonna give you up Never gonna let you down Never gonna run around and desert you Never gonna make you cry Never gonna say goodbye Never gonna tell a lie and hurt you Never gonna give you up Never gonna let you down Never gonna run around and desert you Never gonna make you cry Never gonna say goodbye Never gonna tell a lie and hurt you Never gonna give you up Never gonna let you down Never gonna run around and desert you Never gonna make you cry Never gonna say goodbye";

//Prototypes
static inline double rand01();
static inline void get_dimensions();
static inline void show_info();
static inline void play_animation();
static inline void fill_color();
static inline void setup_global();
static inline void set_colors();
static inline bool init_ui();
static inline void show_matrix(void);
static inline void try_add_drips();
static inline  void update_drips();
static inline  void init_drips();
static inline void matrix_init();
static inline  void fade_n_change_matrix(void);
static inline void matrix_update();
static inline void exit_gracefully(int);

//Functions
static inline int ngyup()
{
	if (counter==strlen(rickroll))
	{
		counter=0;
	}
	if (isprint(rickroll[counter])==0)
	{
		counter++;
		return (int)rickroll[counter+1];
	}
	counter++;
	return (int)rickroll[counter];
}

static inline double rand01()
{
    return (double)rand() /(double)RAND_MAX;
}

static inline void get_dimensions()
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    MAX_X=(int)w.ws_col;
    MAX_Y=(int)w.ws_row;

    matrix =(cell **)malloc(MAX_X * sizeof(cell *));
    for(int i=0;i<MAX_X;i++)
	{
		matrix[i]=(cell*)malloc(MAX_Y*sizeof(cell));
	}
}

//show program information
static inline void show_info()
{
    fprintf(stdout,"[+] DBMatrix by: " GREEN("@whokilleddb")"\n");
    fprintf(stdout,"[+] ITERATIONS: " CYAN("%d")"\n",ITERATIONS);
    fprintf(stdout,"[+] REFRESH_DELAY: " RED("%ld") "\n", REFRESH_DELAY);
    fprintf(stdout,"[+] MAX_INTENSITY: " YELLOW("%d")"\n",MAX_INTENSITY);
    fprintf(stdout,"[+] MIN_INTENSITY: " BLUE("%d")"\n",MIN_INTENSITY);
    fprintf(stdout,"[+] BRIGHTNESS: " MAGENTA("%d")"\n",BRIGHTNESS);    
    fprintf(stdout,"[+] MAX_X: " GREEN("%d")"\n",MAX_X);
    fprintf(stdout,"[+] MAX_Y: " CYAN("%d")"\n",MAX_Y);
    fprintf(stdout,"[+] PROB_DRIP_SPAWN: " YELLOW("%f")"\n", PROB_DRIP_SPAWN);
    fprintf(stdout,"[+] PROB_DIM: " BLUE("%f")"\n", PROB_DIM);
    fprintf(stdout,"[+] PROB_CHANGE: " MAGENTA("%f")"\n", PROB_CHANGE);
    fprintf(stdout,"[+] DDRIP_NUM: " GREEN("%d")"\n", DRIP_NUM);
    fprintf(stdout,"[+] COLOR_MAP: ");
    for(int i=0;i<=MAX_INTENSITY;i++)
    {
        fprintf(stdout, CYAN("%d") " ", color_map[i]);
    }
    fprintf(stdout,"\n");
}

static inline void play_animation()
{
    matrix_update();
    show_matrix();
    usleep(REFRESH_DELAY);
}

static inline void fill_color()
{
    for(int i=0;i<=MAX_INTENSITY;i++)
    {
        if (i==MAX_INTENSITY)
        {
            color_map[i]=BRIGHTNESS;
        }
        else if(i==(MAX_INTENSITY-1))
        {
            color_map[i]=(int)(BRIGHTNESS/2);
        }
        else if(i==0)
        {
            color_map[i]=0;
        }
        else 
        {
            color_map[i]=(int)(round((i*BRIGHTNESS)/MAX_INTENSITY))+1;
        }
    }
}

static inline void setup_global()
{
    fill_color(); //fill color map
    get_dimensions(); //get dynamic struct
}

#ifndef _UI_H_
#define _UI_H_

    static inline void set_colors()
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
        init_color(6, 800,900,800);
        return;
    }

    static inline bool init_ui()
    {
        uiwindow = initscr(); //initialize the ncurses data structures
        if (uiwindow == NULL) return false; //check if window was successfully initialized

        start_color(); // enable use of colors and before any color manipulation function is called
        wbkgd(stdscr, COLOR_PAIR(1));
        if (!has_colors() || !can_change_color() || COLOR_PAIRS < 6)
        {
            perror("[-] " RED("Your Terminal Cannot Run This Program!\n"));
            return false;
        }

        set_colors(); //set custom colors
        return true;
    }

    static void cleanup_ui(void)
    {
        delwin(uiwindow); //Delete Window
        endwin();         
        refresh();        //Refresh Screen
    }

    static inline void show_matrix(void)
    {
        //set the matrix to all black
        for (int X=0; X < MAX_X; X++)
        {
            for (int Y=0; Y < MAX_Y; Y++)
            {
                int intensity=matrix[X][Y].intensity;
                color_set(color_map[intensity], NULL); //sets color you want to apply to your intensity
                mvaddch(Y, X, matrix[X][Y].char_value);
            }
        }
        refresh();
    }

#endif

#ifndef _MATRIX_H_
#define _MATRIX_H_

    static inline void try_add_drips()
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

    static inline  void update_drips()
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

    static inline  void init_drips()
    {
        int i;
        for(i=0;i<DRIP_NUM;i++)
        {
            drips[i].live=false;
        }
    }

    static inline void matrix_init()
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

    static inline  void fade_n_change_matrix(void)
    {
        for (int X=0; X < MAX_X; X++)
        {
            for (int Y=0; Y < MAX_Y; Y++)
            {
                //Randomly change characters
                if (rand01() < PROB_CHANGE || matrix[X][Y].char_value == 0)
                {
                    matrix[X][Y].char_value = ngyup();
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

    static inline void matrix_update()
    {
        if (rand01() < PROB_DRIP_SPAWN)
        {
            try_add_drips();
        }
        update_drips();
        fade_n_change_matrix();
    }

#endif

//Handle SIGINT interrupt signal
static inline void exit_gracefully(int sig) 
{
    cleanup_ui(); //clean User interface

    fprintf(stdout, "[-] Received " MAGENTA("SIGINT\n"));
    fprintf(stdout, "[-] " RED("Exiting") "\n");
}
