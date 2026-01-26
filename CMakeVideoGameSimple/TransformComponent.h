#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "ECS.h"
#include "Vector2D.h"

struct TransformComponent : public Component {
	Vector2D position;
	Vector2D velocity;

	string tag = "";

	int height = 16;
	int width = 16;
	int scale = 1;

	double speed = 1.0;
	double startSpeed = 1.0;

	TransformComponent() = default;

	TransformComponent(double x, double y, string tag = " ") {
		position.x = x;
		position.y = y;
		this->tag = tag;
	}

	TransformComponent(double x, double y, int h, int w, string tag = " ") {
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		this->tag = tag;
	}

	TransformComponent(double x, double y, int h, int w, int scale,
		string tag = " ") {
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		this->scale = scale;
		this->tag = tag;
	}

	TransformComponent(double x, double y, int h, int w, int scale,
	double speed, string tag = " ") {
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		this->scale = scale;
		this->speed = speed;
		this->tag = tag;
		startSpeed = speed;
	}

	void update() override {
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}
};

#endif
