#pragma once
class Environment
{
public:
	static Environment Global;

	Environment();
	~Environment();
	void Draw();

private:
	void DrawGround();
	void DrawSkyBox();
};

