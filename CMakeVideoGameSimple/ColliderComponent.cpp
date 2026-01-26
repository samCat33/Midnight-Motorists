#include "ColliderComponent.h"
#include "Game.h"

void ColliderComponent::init() {
	if (!entity->hasComponent<TransformComponent>()) {
		entity->addComponent<TransformComponent>();
	}


	transform = &entity->getComponent<TransformComponent>();

	width = transform->width * transform->scale;
	height = transform->height * transform->scale;
	collider.w = width;
	collider.h = height;


	Game::colliders.push_back(this);

}

void ColliderComponent::update() {
	collider.x = SetXPos();
	collider.y = SetYPos();
	leftEdge = SetLeftEdge();
	rightEdge = SetRightEdge();
	topEdge = SetTopEdge();
	bottomEdge = SetBottomEdge();

}

void ColliderComponent::draw(){
	//SDL_SetRenderDrawColor(RENDERER, 255, 255, 0, 255);
	//SDL_RenderFillRect(RENDERER, &collider);
}