#include "GameVariables.h"

//GameVariables.h declared these variables

SDL_Renderer* RENDERER = nullptr;

extern bool playerIsAlive = true;
extern bool isRunning = false;
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
int FINISHLINE_SPAWNRATE_MS = 15000;
const int ENEMY_HIT_COOLDOWN = 3000;
int ENEMY_SPAWNX_RANGE = 0;
int ENEMY_SPAWNY_RANGE = 0;

int LINE_WIDTH = WINDOW_W * 0.15;
const double FIRST_DIGIT_FPS = 20;
const double MPH_DIGIT_FPS = 12;

const int STARTING_LIVES = 5;