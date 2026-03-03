#include "GameVariables.h"

//GameVariables.h declared these variables

SDL_Renderer* RENDERER = nullptr;

extern bool playerIsAlive = false;
extern bool gameRunning = false;
extern bool gameStarted = false;
const double DELTA_TIME = 1000/60;
const double GAME_START_DELAY = 4.0;

double currentTime = 0;

const vector<const char*> WINDOW_TITLES = {"I LIKE TO MOVE IT MOVE IT",
"WHO IS IN PARIS?", "RIP TUPAC", "OH YOU DIGGIN IN ME IM NEW BRO IM NEW!",
"THIS IS A CERTIFIED HOOD CLASSIC", 
"WHAT THE HEEEEELLLLL OH MY GOOOOOSH NO WAAYEEAEAEAEAE"};

const vector<double> Y_SPAWNS{0.04, 0.27, 0.52, 0.77};


int WINDOW_W = 0;
int WINDOW_H = 0;
const double WINDOW_W_PROP = 0.6;
const double WINDOW_H_PROP = 0.6;

const char* PLAYER_PATH = "Assets/Player Car.png";
const char* ENEMY_PATH = "Assets/Enemy Car.png";
const char* LINE_PATH = "Assets/Lane Marker.png";
const char* EDGE_PATH = "Assets/Road Barriers.png";
const char* WASD_PATH = "Assets/WASD.png";
const char* LAP_PATH = "Assets/Lap.png";
const char* MPH_PATH = "Assets/Mph.png";
const char* FINISH_LINE_PATH = "Assets/Finish Line.png";
const char* BIG_TEXT_PATH = "Assets/Big Text.png";
const char* FONT_PATH = "Assets/Midnight Motorists Font.png";
const char* ATARI_LINE_PATH = "Assets/Atari Line.png";
const char* GAME_OVER_PATH = "Assets/Game Over.png";
const char* CURSOR_PATH = "Assets/Cursor.png";
const char* RESTART_BUTTON_PATH = "Assets/Restart.png";
const char* TITLE_PATH = "Assets/Title Screen.png";
const char* MENU_BUTTONS_PATH = "Assets/Buttons.png";
const char* GAME_MUSIC_PATH = "Assets/Music.wav";
const char* MAIN_MENU_MUSIC_PATH = "Assets/Main Menu Music.wav";

const int RESTART_BUTTON_X_OFFSET = 172;
const int MENU_BUTTONS_WIDTH = 87;
const int MENU_BUTTONS_HEIGHT = 23;
const int MENU_BUTTONS_X_OFFSET = 14;
const int MENU_BUTTONS_Y_OFFSET = 6;
const int NUM_OF_MENU_BUTTONS = 6;

const int BIG_TEXT_SHIFT = 225;
const int FONT_SHIFT = 24;

const string PLAYER_TAG = "player";
const string ENEMY_TAG = "enemy";
const string MAP_TAG = "map";
const string FINISH_LINE_TAG = "finish";

const int PLAYER_H = 100;
const int PLAYER_W = 100;
const double PLAYER_SPEED = 2.5;
const int ENEMY_H = 100;
const int ENEMY_W = 100;
const int ENEMY_SPEED_RANGE = 4;

const int MAX_MAP_SPEED = 10;

Vector2D obstacleVector(-1,0);
const double STARTING_SPEED = 3.0;

const int PLAYER_HITBOX_H = 100;
const int PLAYER_HITBOX_W = 100;
const int ENEMY_HITBOX_H = 100;
const int ENEMY_HITBOX_W = 100;

int ENEMY_SPAWNRATE_MS = 3000;
const int START_ENEMY_MS = 3000;
const int LAP_SPAWN_RATE = 8000;
const int ENEMY_HIT_COOLDOWN = 3000;
int ENEMY_SPAWNX_RANGE = 0;
int ENEMY_SPAWNY_RANGE = 0;

int DIFFICULTY_MODIFIER = 2;

int LINE_WIDTH = WINDOW_W * 0.15;
const double FIRST_DIGIT_FPS = 20;
const double MPH_DIGIT_FPS = 12;

const int STARTING_LIVES = 3;