#include "stdafx.h"
#include "Vector3f.h"

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

Vector3f Vector3f::Normalised()
{
	Vector3f normalised;
	float length = GetLength();
	normalised.X = X / length;
	normalised.Y = Y / length;
	normalised.Z = Z / length;
	return normalised;
}

string Vector3f::ToString()
{
	return string(to_string(X) + ", " + to_string(Y) + ", " + to_string(Z));
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

Vector3f::~Vector3f()
{}
