#ifndef collidercomponent_h
#define collidercomponent_h

#include "SDL.h"
#include "TransformComponent.h"
#include "GameVariables.h"
using namespace std;

class ColliderComponent : public Component {
	private:
		int height = 0;
		int width = 0;
		int xOffset = 0;
		int yOffset = 0;

		int SetXPos(){
			int x = (transform->position.x) + ( ((transform->width * 
			transform->scale) / 2.0) - (width / 2.0)) + xOffset;
			return x;
		}
		int SetYPos(){
			int y = (transform->position.y) + (((transform->height *
				transform->scale) / 2.0) - (height / 2.0)) + yOffset;
			return y;
		}
		int SetTopEdge(){
			return collider.y;
		}
		int SetBottomEdge(){
			return collider.y + height;
		}
		int SetLeftEdge(){
			return collider.x;
		}
		int SetRightEdge(){
			return collider.x + width;
		}

	public:
		SDL_Rect collider = {0,0,0,0};

		int leftEdge = 0;
		int rightEdge = 0;
		int topEdge = 0;
		int bottomEdge = 0;
		bool hittable = true;
		bool collidable = true;

		TransformComponent* transform = nullptr;

		ColliderComponent():height(0), width(0), xOffset(0), yOffset(0){};
		
		ColliderComponent(int height, int width, bool collidable = true):
		height(height), width(width), xOffset(0), yOffset(0),
		hittable(collidable){};
		
		ColliderComponent(int height, int width, int xOffset, int yOffset,
		bool collidable = true) :
		height(height), width(width), xOffset(xOffset), yOffset(yOffset),
		hittable(collidable){};

		void init() override;


		void update() override;

		void draw() override;
};

#endif
