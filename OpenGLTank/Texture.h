#pragma once
class Texture
{
public:
	GLubyte Image[64][64][3];

	static Texture GroundTexture;

	Texture();

	static void InitialiseAllGlobal();
	void Initialise(std::vector<std::vector<ColorInt>> colors);

	~Texture();

private:
	static void initialiseGroundTexture();
};

