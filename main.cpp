#include <iostream>
#include <stdlib.h>
#include <string>
#include <ctime>

#ifdef __linux__
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>
    #include <SDL2/SDL_mixer.h>
    #include "data/img/backs.xpm"
#else
    #include <SDL.h>
    #include <SDL_image.h>
    #include <SDL_mixer.h>
    #include "data\img\backs.xpm"
#endif

#include "logic.h"

#ifdef __linux__
	const std::string IMG_PATH = "data/img/";
	const std::string SND_PATH = "data/snd/";
#else
	const std::string IMG_PATH = "data\\img\\";
	const std::string SND_PATH = "data\\snd\\";
#endif

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int START_POS_X = 210;
const int START_POS_Y = 21;

const int NEXT_POS_X = 516;
const int NEXT_POS_Y = 95;

const int SCORE_POS_X = 473;
const int SCORE_POS_Y = 232;

const int SCORE_LOSE_POS_X = 250;
const int SCORE_LOSE_POS_Y = 273;

const int NUM_SIZE = 20;

int speed = 310;
int delta_speed = 10;

SDL_Window* win = NULL; 
SDL_Surface* srf = NULL;

SDL_Surface* back = NULL;

Mix_Chunk* moveSound = NULL;
Mix_Chunk* pauseSound = NULL;
Mix_Chunk* joinSound = NULL;
Mix_Chunk* rotateSound = NULL;
Mix_Chunk* gameoverSound = NULL;

Mix_Music* introSound = NULL;
Mix_Music* mainSound = NULL;

std::pair<SDL_Surface*, int> figures[240];

void draw_level(int level)
{
    std::string lvl = std::to_string(level);

    while(lvl.length() != 2)
        lvl = "0" + lvl;

    for(int i = 0; i < lvl.length(); i++)
    {
        std::string name = "";
        name.push_back(lvl[i]);
        name += ".png";

        SDL_Surface* lvlo = IMG_Load((IMG_PATH+name).c_str());
        SDL_Rect coord;

        coord.x = NUM_SIZE*i + 575;
        coord.y = 256;

        SDL_BlitSurface(lvlo, NULL, srf, &coord);
        SDL_UpdateWindowSurface(win);

        SDL_FreeSurface(lvlo);
    }
}

void draw_scores(int scores, bool loseScreen = false)
{
    std::string sc = std::to_string(scores);

    while(sc.length() != 7)
        sc = "0" + sc;

    for(int i = 0; i < sc.length(); i++)
    {
        std::string name = "";
        name.push_back(sc[i]);
        name += ".png";

        SDL_Surface* sco = IMG_Load((IMG_PATH+name).c_str());
        SDL_Rect coord;

        if(loseScreen)
        {
            coord.x = NUM_SIZE*i + SCORE_LOSE_POS_X;
            coord.y = SCORE_LOSE_POS_Y;
        }

        else
        {
            coord.x = NUM_SIZE*i + SCORE_POS_X;
            coord.y = SCORE_POS_Y;
        }

        SDL_BlitSurface(sco, NULL, srf, &coord);
        SDL_UpdateWindowSurface(win);

        SDL_FreeSurface(sco);
    }
}

void draw_field(Field& field)
{
    int k = 0;
	for(int i(4); i < 24; i++)
	{
		for(int j(0); j < 10; j++)
		{	
            if(figures[k].first != NULL && figures[k].second == field[i][j])
            {
                k++;
                continue;
            }

			std::string fig_name = std::to_string(field[i][j]) + "_fig.png";
			figures[k].first = IMG_Load((IMG_PATH+fig_name).c_str());
            figures[k].second = field[i][j];

			SDL_Rect coord;
			coord.x = 22*j+START_POS_X;
			coord.y = 22*(i-4)+START_POS_Y;

			SDL_BlitSurface(figures[k].first, NULL, srf, &coord);
			SDL_UpdateWindowSurface(win);

            SDL_FreeSurface(figures[k].first);

            k++;
		}
	}
}

void draw_next(Field nextf)
{
    std::string fignum = "";

    if(nextf[0][0] != 0)
        fignum = std::to_string(nextf[0][0]);
    else
        fignum = std::to_string(nextf[0][1]);

    std::string fig_name = fignum + "_next.png";

    SDL_Surface* fig = IMG_Load((IMG_PATH+fig_name).c_str());

    SDL_Rect coord;
    coord.x = 518;
    coord.y = 91;

    SDL_BlitSurface(fig, NULL, srf, &coord);
    SDL_UpdateWindowSurface(win);

    SDL_FreeSurface(fig);
}

bool init()
{
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
		return false;
	if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    	return false;
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        return false;

	win = SDL_CreateWindow("Tetris",SDL_WINDOWPOS_UNDEFINED, 
    	SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 
    	SDL_WINDOW_SHOWN);
	
	if(win == NULL)
		return false;

	srf = SDL_GetWindowSurface(win);

	return true;
}

void quit() 
{    
    SDL_DestroyWindow(win);

    SDL_Quit();
}

void reset()
{
	for(int i = 0; i < 240; i++)
	{
		figures[i].first = NULL;
		figures[i].second = 0;
	}
}

void game()
{
   	Game tetris;
    int start_time = SDL_GetTicks();

    SDL_FillRect(srf, NULL, SDL_MapRGB(srf->format, 0, 0, 0));

    moveSound = Mix_LoadWAV((SND_PATH+"moving.wav").c_str());
    pauseSound = Mix_LoadWAV((SND_PATH+"pause.wav").c_str());
    rotateSound = Mix_LoadWAV((SND_PATH+"rotate.wav").c_str());
    joinSound = Mix_LoadWAV((SND_PATH+"join.wav").c_str());

    back = IMG_ReadXPMFromArray(back_xpm);

    tetris.MakeMove();

    bool run = true;
    bool lose = false;
    bool paused = false;

    SDL_Event e;

    while(run)
    {
    	while(SDL_PollEvent(&e) != 0)
    	{
            if (e.type == SDL_QUIT)
            {
                run = false;
            }

            if(e.type == SDL_KEYDOWN)
            {
                if(e.key.keysym.sym == SDLK_p)
                {
                    Mix_PlayChannel(-1,pauseSound, 0);
            
                    paused = !paused; 

                    if(paused)
                    {
                        back = IMG_ReadXPMFromArray(back4_xpm);
                        SDL_BlitSurface(back, NULL, srf, NULL);
                        SDL_UpdateWindowSurface(win);
                    }

                    else
                    {
                        back = IMG_ReadXPMFromArray(back_xpm);
                    }
                }

                if(paused)
                    continue;

                if(e.key.keysym.sym != SDLK_UP)
                    Mix_PlayChannel(-1, moveSound, 0);

                if(e.key.keysym.sym == SDLK_LEFT)
                {
                    tetris.MoveLeft();
                }

                else if(e.key.keysym.sym == SDLK_RIGHT)
                {
                    tetris.MoveRight();
                }

                else if(e.key.keysym.sym == SDLK_UP)
                {
                    Mix_PlayChannel(-1,rotateSound, 0);
                    tetris.Rotate();
                }

                else if(e.key.keysym.sym == SDLK_DOWN)
                {
                    while(tetris.AbleToMove(Direction::down))
                        tetris.MakeMove();
                }

                else if(e.key.keysym.sym == SDLK_KP_PLUS)
                {
                    tetris.IncreaseLevel();
                }
            }
        }

        if(paused)
            continue;

        if(SDL_GetTicks() > start_time + (speed-tetris.GetLevel()*delta_speed))
        {
            if(tetris.deletelines)
            {
                tetris.deletelines = false;
            }

            start_time = SDL_GetTicks();
            tetris.MakeMove();

            if(!tetris.AbleToMove(Direction::down))
                Mix_PlayChannel(-1,joinSound, 0);

            if(tetris.isLose())
                lose = true;
        }
        
        if(lose)
            break;

        auto f = tetris.GetField();
        auto g = tetris.GetClearField();
        auto nf = tetris.GetNextFigure();

        SDL_BlitSurface(back, NULL, srf, NULL);
        SDL_UpdateWindowSurface(win);

		draw_field(f);
        draw_scores(tetris.GetScore());
        draw_level(tetris.GetLevel());
        draw_next(nf);
    }

    Mix_HaltMusic();

    if(!lose)
    {
        quit();
        return;
    }


    back = IMG_ReadXPMFromArray(back3_xpm);
    SDL_BlitSurface(back, NULL, srf, NULL);
    draw_scores(tetris.GetScore(), true);
    SDL_UpdateWindowSurface(win);

    gameoverSound = Mix_LoadWAV((SND_PATH+"gameover.wav").c_str());

    Mix_PlayChannel(-1,gameoverSound, 0);

    bool reseted = false;

    while(run && lose)
    {
        while(SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                run = false;
            }

            else if(e.type == SDL_KEYDOWN)
            {
            	if(e.key.keysym.sym == SDLK_SPACE)
            	{
            		Mix_HaltChannel(-1);
            		reset();
            		reseted = true;
            		break;
            	}
            }
        }

        if(reseted)
        	break;
    }

    if(reseted)
    	game();
    quit();
}

int main(int argc, char** argv)
{
	if(!init())
		return -1;

    srand(time(NULL));

    bool run = true;
    bool isintro = true;

    SDL_Surface* icon = IMG_Load((IMG_PATH+"favicon.png").c_str());
    SDL_SetWindowIcon(win, icon);

    SDL_Event e;

    back = IMG_ReadXPMFromArray(back1_xpm);
    introSound = Mix_LoadMUS((SND_PATH+"intro.wav").c_str());

    int intro_start = SDL_GetTicks();
    bool intron = false;

    Mix_PlayMusic(introSound, -1);

    while(run && isintro)
    {
    	while(SDL_PollEvent(&e) != 0)
    	{
            if (e.type == SDL_QUIT)
            {
                run = false;
            }

            if(e.type == SDL_KEYDOWN)
            {
            	if(e.key.keysym.sym == SDLK_SPACE)
            		if(isintro)
            		{
            			SDL_FillRect(srf, NULL, 0x000000);
            			back = IMG_ReadXPMFromArray(back_xpm);
            			isintro = false;
            			break;
            		}
            }
        }

        if(!isintro)
            break;

        if(SDL_GetTicks() > intro_start+400)
        {
            intron = !intron;
            intro_start = SDL_GetTicks();
        }

        SDL_FreeSurface(back);

        if(intron)
        	back = IMG_ReadXPMFromArray(back1_xpm);
        else
        	back = IMG_ReadXPMFromArray(back2_xpm);

        SDL_BlitSurface(back, NULL, srf, NULL);
        SDL_UpdateWindowSurface(win);
    }

    Mix_HaltMusic();
    Mix_FreeMusic(introSound);

    if(!run)
    	return 0;

    game();

    return 0;
}