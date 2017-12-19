#include "stdafx.h"
#include "Vector3f.h"
#include <cmath>

Vector3f::Vector3f()
{

}

Vector3f::Vector3f(float x, float y, float z)
{
	X = x;
	Y = y;
	Z = z;
}

Vector3f Vector3f::CalculateNormal(Vector3f p1, Vector3f p2, Vector3f p3)
{
	Vector3f U = p2 - p1;
	Vector3f V = p3 - p1;
	Vector3f result;
	result.X = (U.Y * V.Z) - (U.Z * V.Y);
	result.Y = (U.Z * V.X) - (U.X * V.Z);
	result.Z = (U.X * V.Y) - (U.Y * V.X);
	return result;
}

Vector3f Vector3f::CalculateCenter(Vector3f p1, Vector3f p2, Vector3f p3)
{
	Vector3f result;
	//result.X = (min(min(p1.X, p2.X), p3.X) + max(max(p1.X, p2.X), p3.X)) / 2;
	//result.Y = (min(min(p1.Y, p2.Y), p3.Y) + max(max(p1.Y, p2.Y), p3.Y)) / 2;
	//result.Z = (min(min(p1.Z, p2.Z), p3.Z) + max(max(p1.Z, p2.Z), p3.Z)) / 2;
	
	//result.X = (p1.X + p2.X + p3.X) / 3;
	//result.Y = (p1.Y + p2.Y + p3.Y) / 3;
	//result.Z = (p1.Z + p2.Z + p3.Z) / 3;

	result = p1 + p2 + p3;
	result = result / 3;

	return result;
}

float Vector3f::GetLength()
{
	float length = sqrt((X * X) +  (Y * Y) + (Z * Z));
	return length;
}

void Vector3f::Normalise()
{
	float length = GetLength();
	X = X / length;
	Y = Y / length;
	Z = Z / length;
}

//Operator behaviours
Vector3f operator+(Vector3f left, Vector3f right)
{
	Vector3f result;
	result.X = left.X + right.X;
	result.Y = left.Y + right.Y;
	result.Z = left.Z + right.Z;
	return result;
}

Vector3f operator-(Vector3f left, Vector3f right)
{
	Vector3f result;
	result.X = left.X - right.X;
	result.Y = left.Y - right.Y;
	result.Z = left.Z - right.Z;
	return result;
}

Vector3f operator*(Vector3f value, float multiplier)
{
	value.X *= multiplier;
	value.Y *= multiplier;
	value.Z *= multiplier;
	return value;
}

Vector3f operator*(float multiplier, Vector3f value)
{
	value.X *= multiplier;
	value.Y *= multiplier;
	value.Z *= multiplier;
	return value;
}

Vector3f operator/(Vector3f value, float divider)
{
	value.X /= divider;
	value.Y /= divider;
	value.Z /= divider;
	return value;
}

Vector3f operator/(float divider, Vector3f value)
{
	value.X /= divider;
	value.Y /= divider;
	value.Z /= divider;
	return value;
}


Vector3f::~Vector3f()
{}
