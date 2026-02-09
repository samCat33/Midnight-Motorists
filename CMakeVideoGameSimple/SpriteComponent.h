#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include "ECS.h"
#include "TransformComponent.h"
#include "Game.h"
#include "GameVariables.h"
#include "SDL.h"
#include "TextureManager.h"


class SpriteComponent : public Component {
	
	private:

		TransformComponent *transform = nullptr;
		SDL_Texture *texture = nullptr;
		SDL_Rect destRect = {0,0,0,0};
		SDL_Rect srcRect = { 0,0,0,0 };

		int width;
		int height;
		bool animated = false;
		bool dying = false;
		bool rotated = false;
		double angle = 0;
		int srcRectW = 0;
		int srcRectH = 0;
		int srcRectY = 0;
		int frames = 1;
		double fps = 1;
		int oneThousandOverFPS = 1;

	public:
		int iterations = 0;
		double resetTime = 0;
		int customIterations = 0;
		bool iterating = true;
		bool hidden = false;
		bool test = false;

		void setTexture(const char* path) {
			texture = TextureManager::LoadTexture(path);
		}
		
		SpriteComponent() = default;

		SpriteComponent(const char* path) {
			setTexture(path);
		}

		SpriteComponent(const char* path, double angle){
			setTexture(path);
			rotated = true;
			this->angle = angle;
		}

		SpriteComponent(const char* path, double angle, int frames,
		double fps, int srcRectW = 0, int srcRectH = 0,
		int srcRectY = 0) {
			setTexture(path);
			rotated = true;
			animated = true;
			this->angle = angle;
			this->frames = frames;
			this->fps = fps;
			this->srcRectW = srcRectW;
			this->srcRectH = srcRectH;
			this->srcRectY = srcRectY;
;		}

		//Deallocate textures
		~SpriteComponent(){
			if (texture != nullptr) {
				SDL_DestroyTexture(texture);
			}
		}


		//verified
		void init() override{
			if (!entity->hasComponent<TransformComponent>()) {
				entity->addComponent<TransformComponent>();
			}

			transform = &entity->getComponent<TransformComponent>();
			srcRect.w = (srcRectW < 1) ? transform->width : srcRectW;
			srcRect.h = (srcRectH < 1) ? transform->height : srcRectH;
			srcRect.y = (srcRectY < 1) ? 0 : srcRectY;
			
			width = srcRect.w;
			height = srcRect.h;

			if (fps > 0) {
				oneThousandOverFPS = 1000 / fps;
			}
			else {
				oneThousandOverFPS = 1;
			}

		}

		void update() override{
			
			//Set the destRect to match the transform components
			destRect.x = transform->position.x;
			destRect.y = transform->position.y;

			destRect.w = transform->width * transform->scale;
			destRect.h = transform->height * transform->scale;

			//Play animations if there are animations while the player
			//is alive
			if (playerIsAlive && animated) {

				if (iterating) {

					if (fps >0) {
						iterations =
							((currentTime - resetTime) / oneThousandOverFPS)
							+ customIterations;
					}
					else {
						iterations = customIterations;
					}
				

					//Used if we wanted to test a specific entity with
					//a sprite component
					if (test) {
						showSrcRectStats();
					}

					if (frames > 0) {
						srcRect.x = srcRect.w * (iterations % frames);
					}

				}
			}
		}

		//This is used in render() to render the sprite onto the screen
		void draw() override{
			TextureManager::Draw(texture, srcRect, destRect, angle);
		}

		void hide() {
			srcRect.w = 0;
			srcRect.h = 0;
			hidden = true;
		}


		void show() {	
			srcRect.w = width;
			srcRect.h = height;
			hidden = false;
		}

		void changeSrcRectW(int srcRectW) {
			srcRect.w = srcRectW;
			width = srcRectW;
		}
		void changeSrcRectX(int srcRectX) {
			srcRect.x = srcRectX;
		}
		void showSrcRectStats() {
			cout << "Src Rect W: " << srcRect.w << endl;
			cout << "Src Rect H: " << srcRect.h << endl;
			cout << "Src Rect X: " << srcRect.x << endl;
		}

		SDL_Rect& getDestRect() {
			return destRect;
		}
};

#endif
