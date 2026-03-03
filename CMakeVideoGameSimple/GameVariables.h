#ifndef GAME_VARIABLES_H
#define GAME_VARIABLES_H

#include <SDL.h>
#include <vector>
#include <string>
#include "Vector2D.h"
using namespace std;

//Variables for tweaking game functionality declared here (used globally)
//Defined in GameVariables.cpp

extern SDL_Renderer* RENDERER; 


extern bool playerIsAlive;
extern bool gameRunning;
extern bool gameStarted;

extern int WINDOW_W;
extern int WINDOW_H;
extern const double GAME_START_DELAY;
extern const double DELTA_TIME;

extern double currentTime;

extern const vector<const char*> WINDOW_TITLES;
extern const vector<double> Y_SPAWNS;

extern const double WINDOW_W_PROP;
extern const double WINDOW_H_PROP;

extern const char* PLAYER_PATH;
extern const char* ENEMY_PATH;
extern const char* LINE_PATH;
extern const char* EDGE_PATH;
extern const char* WASD_PATH;
extern const char* LAP_PATH;
extern const char* MPH_PATH;
extern const char* FINISH_LINE_PATH;
extern const char* BIG_TEXT_PATH;
extern const char* FONT_PATH;
extern const char* ATARI_LINE_PATH;
extern const char* GAME_OVER_PATH;
extern const char* CURSOR_PATH;
extern const char* RESTART_BUTTON_PATH;
extern const char* TITLE_PATH;
extern const char* MENU_BUTTONS_PATH;
extern const char* GAME_MUSIC_PATH;
extern const char* MAIN_MENU_MUSIC_PATH;

extern const int RESTART_BUTTON_X_OFFSET;
extern const int MENU_BUTTONS_WIDTH;
extern const int MENU_BUTTONS_HEIGHT;
extern const int MENU_BUTTONS_X_OFFSET;
extern const int MENU_BUTTONS_Y_OFFSET;
extern const int NUM_OF_MENU_BUTTONS;

extern const int BIG_TEXT_SHIFT;
extern const int FONT_SHIFT;

extern const string PLAYER_TAG;
extern const string ENEMY_TAG;
extern const string MAP_TAG;
extern const string FINISH_LINE_TAG;

extern const int PLAYER_H;
extern const int PLAYER_W;
extern const double PLAYER_SPEED;
extern const int ENEMY_H;
extern const int ENEMY_W;
extern const int ENEMY_SPEED_RANGE;

extern const int MAX_MAP_SPEED;

extern Vector2D obstacleVector;
extern const double STARTING_SPEED;

extern const int PLAYER_HITBOX_H;
extern const int PLAYER_HITBOX_W;
extern const int ENEMY_HITBOX_H;
extern const int ENEMY_HITBOX_W;

extern int ENEMY_SPAWNRATE_MS;
extern const int START_ENEMY_MS;
extern const int LAP_SPAWN_RATE;
extern const int ENEMY_HIT_COOLDOWN;
extern int ENEMY_SPAWNX_RANGE;
extern int ENEMY_SPAWNY_RANGE;

extern int DIFFICULTY_MODIFIER;

extern int LINE_WIDTH;
extern const double FIRST_DIGIT_FPS;
extern const double MPH_DIGIT_FPS;

extern const int STARTING_LIVES;

#endif
