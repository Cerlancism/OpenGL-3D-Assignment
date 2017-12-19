#pragma once
class Vector3f
{
public:

	float X = 0;
	float Y = 0;
	float Z = 0;

	Vector3f();
	Vector3f(float x, float y, float z);

	static Vector3f CalculateNormal(Vector3f p1, Vector3f p2, Vector3f p3);
	static Vector3f CalculateCenter(Vector3f p1, Vector3f p2, Vector3f p3);

	float GetLength();

	void Normalise();

	~Vector3f();
};
//Define vector operator behaviours
Vector3f operator+(Vector3f left, Vector3f right);

Vector3f operator-(Vector3f left, Vector3f right);

Vector3f operator*(Vector3f value, float multiplier);

Vector3f operator*(float multiplier, Vector3f value);

Vector3f operator/(Vector3f value, float divider);

Vector3f operator/(float divider, Vector3f value);

