#ifndef keyboard_controller_h
#define keyboard_controller_h

#include "Game.h"
#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "GameVariables.h"
#include <cmath>

struct KeyboardController : public Component {
	TransformComponent *transform;
	double x;
	double y;
	Uint32 timer;

	void init() override{
		transform = &entity->getComponent<TransformComponent>();
	}

	void update() override {
		timer = currentTime;

		if (playerIsAlive) {
			const Uint8* state = SDL_GetKeyboardState(NULL);

			x = 0;
			y = 0;

			if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP]) y = -1;
			if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN]) y = 1;
			if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT]) x = -1;
			if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]) x = 1;

			y *= transform->speed;
			x *= transform->speed;


			//Normalize vector
			if (y != 0 && x != 0) {
				Vector2D normalizedVector(x * (sqrt(2.0) / 2.0), y * (sqrt(2.0) / 2.0));
				transform->velocity.x = normalizedVector.x;
				transform->velocity.y = normalizedVector.y;
			}
			else {
				transform->velocity.x = x;
				transform->velocity.y = y;
			}

		}
	}
};

#endif