#include "ECS.h"
#include "SpriteComponent.h"
#include "GameVariables.h"

#ifndef BUTTON_H
#define BUTTON_H

struct ButtonComponent : public Component {

	TransformComponent* transform = nullptr;
	SpriteComponent* sprite = nullptr;
	SDL_Rect* mouseRect = nullptr;
	SDL_Rect* buttonRect = nullptr;
	int* x = nullptr;
	int* y = nullptr;
	bool isSelected = false;
	int selectedX = 0;

	ButtonComponent(SDL_Rect* mouseRect, int selectedX) {
		this->mouseRect = mouseRect;
		this->selectedX = selectedX;
	}

	~ButtonComponent(){};


	void init() override {
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>();
		}
		if (!entity->hasComponent<SpriteComponent>()) {
			entity->addComponent<SpriteComponent>();
		}

		transform = &(entity->getComponent<TransformComponent>());
		sprite = &(entity->getComponent<SpriteComponent>());
		buttonRect = &(sprite->getDestRect());
		x = &(mouseRect->x);
		y = &(mouseRect->y);
	}

	void update() override {

		if (!sprite->hidden) {
			SDL_GetMouseState(x, y);
			if (SDL_HasIntersection(mouseRect, buttonRect)) {
				isSelected = true;
				sprite->changeSrcRectX(selectedX);
			}
			else {
				isSelected = false;
				sprite->changeSrcRectX(0);
			}
		}
		else {
			isSelected = false;
			sprite->changeSrcRectX(0);
		}
	}
};


#endif