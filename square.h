#pragma once
class Square
{
public:
	float left;
	float top;
	float width;
	float height;
	bool isSelected;
	void Draw();
	void HandleLButtonDown(float xpos,float ypos);
	void HandleMouseMove(float xpos, float ypos);

	Square();
	~Square();
};

