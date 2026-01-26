#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <vector>
#include <iostream>
using namespace std;

struct Vector2D {
	double x;
	double y;
	Vector2D() : x(0), y(0){};
	Vector2D(double x, double y) : x(x), y(y){};

	Vector2D& Add(const Vector2D& vector2);
	Vector2D& Subtract(const Vector2D& vector2);
	Vector2D& Multiply(const Vector2D& vector2);
	Vector2D& Divide(const Vector2D& vector2);

	friend Vector2D& operator+ (Vector2D& vec1, const Vector2D& vec2);
	friend Vector2D& operator- (Vector2D& vec1, const Vector2D& vec2);
	friend Vector2D& operator* (Vector2D& vec1, const Vector2D& vec2);
	friend Vector2D& operator/ (Vector2D& vec1, const Vector2D& vec2);

	Vector2D& operator+= (const Vector2D& vec2);
	Vector2D& operator-= (const Vector2D& vec2);
	Vector2D& operator*= (const Vector2D& vec2);
	Vector2D& operator/= (const Vector2D& vec2);

	Vector2D& operator*= (const int i);

	Vector2D& Zero();

	friend ostream& operator<< (ostream& out, const Vector2D& vec);
};

#endif