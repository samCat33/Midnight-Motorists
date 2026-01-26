#ifndef collision_h
#define collision_h

#include "SDL.h"
#include "ColliderComponent.h"

struct Collision {
	static bool AABB(const SDL_Rect& rect1, const SDL_Rect& rect2) {
		bool flag = false;
		if (rect1.x + rect1.w >= rect2.x &&
			rect1.y + rect1.h >= rect2.y &&
			rect2.x + rect2.w >= rect1.x &&
			rect2.y + rect2.h >= rect1.y) {
			flag = true;
		}
		return flag;
	}

	static bool AABB(const ColliderComponent& colliderA,
		const ColliderComponent& colliderB) {

		bool flag = false;

		if (AABB(colliderA.collider, colliderB.collider)) {
			flag = true;
		}

		return flag;
	}
};

#endif
