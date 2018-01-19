#include "stdafx.h"
#include "Texture.h"

Texture Texture::GroundTexture;

Texture::Texture()
{}

void Texture::InitialiseAllGlobal()
{
	initialiseGroundTexture();
}

void Texture::Initialise(vector<vector<ColorInt>> colors)
{
	int nrOfCheckersOnRow = colors.size();
	int dim = 64 / nrOfCheckersOnRow;
	int red = 0;
	int green = 0;
	int blue = 0;
	int count = 0;
	for (int i = 0; i < nrOfCheckersOnRow; i++)
	{
		for (int j = 0; j < nrOfCheckersOnRow; j++)
		{
			int row = (i);
			int col = (j);
			red = colors[row][col].Red;
			green = colors[row][col].Green;
			blue = colors[row][col].Green;
			for (int k = 0; k < dim; k++)
			{
				for (int l = 0; l < dim; l++)
				{
					Image[k + dim * i][l + dim * j][0] = (GLubyte)red;
					Image[k + dim * i][l + dim * j][1] = (GLubyte)green;
					Image[k + dim * i][l + dim * j][2] = (GLubyte)blue;
				}
			}
		}
	}
}

void Texture::initialiseGroundTexture()
{
	ColorInt light{ 151, 108 , 74 };
	ColorInt dark{ 100, 80, 40 };
	vector<vector<ColorInt>> colors;
	int divisions = 64;
	for (int i = 0; i < divisions; i++)
	{
		colors.push_back(vector<ColorInt>(divisions));
		for (int j = 0; j < divisions; j++)
		{
			colors[i][j] = (rand() % 2) == 0 ? light : dark;
		}
	}
	GroundTexture.Initialise(colors);
}

Texture::~Texture()
{}
