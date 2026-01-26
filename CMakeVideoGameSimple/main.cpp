#include <SDL.h>
#include <SDL_mixer.h>
#include "Game.h"
#include "GameVariables.h"
#include "TextureManager.h"


int main(int argc, char* argv[]) {

	SDL_DisplayMode displayMode = {};
	SDL_Window* window = nullptr;
	SDL_Event event = {};
	int screenWidth = 0;
	int screenHeight = 0;
	srand(time(NULL));

	//If SDL cannot start
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "Could not start up SDL!" << endl;
	}

	//If display mode could not be obtained
	if (SDL_GetDesktopDisplayMode(0, &displayMode) != 0) {
		cout << "Could not get display mode!" << endl;
	}

	/* Set our WINDOW_W and WINDOW_H variables to 
	*  represent the width and height of the user's screen
	*  scaled according to the proportion of the screen
	*  taken up by the game  */
	screenWidth = displayMode.w;
	screenHeight = displayMode.h;
	WINDOW_W = screenWidth * WINDOW_W_PROP;
	WINDOW_H = screenHeight * WINDOW_H_PROP;
	cout << "WINDOW WIDTH: " << WINDOW_W << endl;
	cout << "WINDOW HEIGHT: " << WINDOW_H << endl << endl << endl;

	window = SDL_CreateWindow(WINDOW_TITLES.at(rand() % WINDOW_TITLES.size()),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_W, WINDOW_H,
		SDL_WINDOW_RESIZABLE);

	ENEMY_SPAWNX_RANGE = WINDOW_W / 0.5;
	ENEMY_SPAWNY_RANGE = WINDOW_H - (0.2 * WINDOW_H);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	RENDERER = SDL_CreateRenderer(window, -1, 0);
	
	Game game;

	game.window = window;
	game.init();
	
	//This is the core of where everything runs
	while (isRunning) {
		game.runGame();
	}

	game.clean();
	
	return 0;
}