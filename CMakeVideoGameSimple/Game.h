#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <vector>
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "Collision.h"
#include "KeyboardController.h"
#include "GameVariables.h"
#include "TextureManager.h"

class ColliderComponent;


struct Game
{
    Game(){};
    ~Game(){};
    SDL_Window* window = nullptr;
    SDL_Event event = {};
    int score = 0;
    int frameTime = 0;
    static int numFrames;
    static SDL_TimerID timerID;
    static vector<ColliderComponent*> colliders;

    int x = 0;
    int y = 0;
    int lane = 0;
    double timeInterval = 0;

    SDL_Texture* carTexture = nullptr;

    void init();
    void runGame();
    void handleEvents();
    void update();
    void render();
    void clean();


};

#endif
