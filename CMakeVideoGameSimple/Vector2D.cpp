#include "Vector2D.h"

Vector2D& Vector2D::Add(const Vector2D& vector2) {
	this->x += vector2.x;
	this->y += vector2.y;
	return *this;
}

Vector2D& Vector2D::Subtract(const Vector2D& vector2) {
	this->x -= vector2.x;
	this->y -= vector2.y;
	return *this;
}

Vector2D& Vector2D::Multiply(const Vector2D& vector2) {
	this->x *= vector2.x;
	this->y *= vector2.y;
	return *this;
}

Vector2D& Vector2D::Divide(const Vector2D& vector2) {
	this->x /= vector2.x;
	this->y /= vector2.y;
	return *this;
}

Vector2D& operator+ (Vector2D& vec1, const Vector2D& vector2) {
	return vec1.Add(vector2);
}

Vector2D& operator- (Vector2D& vec1, const Vector2D& vector2) {
	return vec1.Subtract(vector2);
}

Vector2D& operator* (Vector2D& vec1, const Vector2D& vector2) {
	return vec1.Multiply(vector2);
}

Vector2D& operator/ (Vector2D& vec1, const Vector2D& vector2) {
	return vec1.Divide(vector2);
}


Vector2D& Vector2D::operator+= (const Vector2D& vector2) {
	return this->Add(vector2);
}

Vector2D& Vector2D::operator-= (const Vector2D& vector2) {
	return this->Subtract(vector2);
}

Vector2D& Vector2D::operator*= (const Vector2D& vector2) {
	return this->Multiply(vector2);
}

Vector2D& Vector2D::operator/= (const Vector2D& vector2) {
	return this->Divide(vector2);
}

Vector2D& Vector2D::Zero() {
	this->x = 0;
	this->y = 0;
	return *this;
}

Vector2D& Vector2D::operator*= (const int i) {
	x *= i;
	y *= i;
	return *this;
}

ostream& operator<< (ostream& out, const Vector2D& vec) {
	out << "(" << vec.x << "," << vec.y << ")" << endl;
	return out;
}