// IGRA_LAB.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "IGRA_LAB.h"

//For IGRA - OPENGL
#include <gl\GL.h>
#include <gl\GLU.h>
#include <math.h>
#include <cmath>
#include <stdio.h>
#include <vector>
#include "Vector3f.h"
#include "Clock.h"
using namespace std;

#include "square.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;					 // current instance
WCHAR szTitle[MAX_LOADSTRING];		 // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING]; // the main window class name

// Added for IGRA
int width = 400;
int height = 400;
HWND hWnd = NULL;
HDC hDC = NULL;
HGLRC hRC = NULL;

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

GLuint PixelFormat;

// The position of the jumping circle
float xPosShape = 0.0;
float yPosShape = 0.0;

GLUquadric *mySpere;
GLUquadric *myCylinder;

GLubyte image[64][64][3];
GLubyte ColouredRectanglesTexture[64][64][3];

Clock *clock;

// The current rotation angle of the cube
float yRot = 0;
// We assume cube takes 10 seconds to make a full rotation
double rotationalVelicityInDegreesPerSeconds = 0.0;

int InitOpenGL();
void DrawSphere(float radius);
void DrawCylinder();
void DrawDumbbell();
void DrawSpheresonAxis(float startsize, int iterations);
void DrawPyramidCircle(int count, float distance, float scale);
void DrawSurrealSphere(int layers);
void DrawGLScene();
void Draw2D_AxisSystem();
void Draw3D_AxisSystem();
void DrawPyramid();
void DrawNormal(float xN, float yN, float zN, float sX, float sY, float sZ);
void DrawPyramid(int config);
void DrawBlockySpiral(int nrOfLines);
void ReSizeGLScene(GLsizei width, GLsizei height);
void DrawCircle(float r, float xc, float yc, int nrOfPoints);
void DrawSineCurves(float startX, float startY, float nrOfSineCurves, float scale);
void convertWindowToOpenGLCoordinates(int xWinPos, int yWinPos, float &xOpenGLPos, float &yOpenGLPos);

void CreateCheckerBoardTexture()
{
	int nrOfCheckersOnRow = 8;
	float dim = 64.0 / nrOfCheckersOnRow;
	int red = 0;
	int green = 0;
	int blue = 0;
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			// Calculate in which checkerboard
			//rectangle the pixel falls
			int row = (int)(i / dim);
			int col = (int)(j / dim);
			int c = 0;
			if (row % 2 == 0)
			{ // Even rows start with black
				if (col % 2 == 0)
				{
					// All even column will be black
					red = green = blue = 0;
				}
				else
				{
					green = blue = 0;
					red = 255;
				}
			}
			else
			{
				// Odd rows start with red
				if (col % 2 == 0)
				{
					// All even column will be red
					green = blue = 0;
					red = 255;
				}
				else
				{
					red = green = blue = 0;
				}
			}
			// Drawing a green border around the image
			if (i == 0 || i == 63 || j == 0 || j == 63)
			{
				red = 0;
				green = 255;
				blue = 0;
			}
			image[i][j][0] = (GLubyte)red;
			image[i][j][1] = (GLubyte)green;
			image[i][j][2] = (GLubyte)blue;
		}
	}
}

void CreateColouredRectanglesTexture()
{
	int nrOfCheckersOnRow = 8;
	float dim = 64.0 / nrOfCheckersOnRow;
	int red = 0;
	int green = 0;
	int blue = 0;
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			// Calculate in which checkerboard
			//rectangle the pixel falls
			int row = (int)(i / dim);
			int col = (int)(j / dim);
			int red = 255;
			int green = 255;
			int blue = 0;
			if ((i >= 16 && i <= 23) || (i >= 39 && i <= 46) || (j >= 16 && j <= 23) || (j >= 39 && j <= 46))
			{
				red = 0;
				green = 0;
				blue = 255;
			}

			if ((i >= 8 && i <= 15) || (i >= 47 && i <= 54) || (j >= 8 && j <= 15) || (j >= 47 && j <= 54))
			{
				red = 0;
				green = 255;
				blue = 0;
			}
			if ((i >= 1 && i <= 7) || (i >= 55 && i <= 62) || (j >= 1 && j <= 7) || (j >= 55 && j <= 62))
			{
				red = 255;
				green = 0;
				blue = 0;
			}
			// Drawing a green border around the image
			if (i == 0 || i == 63 || j == 0 || j == 63)
			{
				red = 0;
				green = 0;
				blue = 0;
			}
			ColouredRectanglesTexture[i][j][0] = (GLubyte)red;
			ColouredRectanglesTexture[i][j][1] = (GLubyte)green;
			ColouredRectanglesTexture[i][j][2] = (GLubyte)blue;
		}
	}
}

void DrawTexturedTriangle()
{

	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
				 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D,
					GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D,
					GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D,
					GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D,
					GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// Specify an indent between top two vertices
	// If indent is 1 then we have a square
	// If indent is 0 then we have a triangle
	float indent = 0;	//Try these values 0.75, 0.5, 0.25, 0
	glBegin(GL_POLYGON); // 0, 1, 2, 3
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, -1.0, 0); // v0 Left Bottom
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-indent, 1.0, 0); // v1 Left-Centre Top
	glTexCoord2f(0.5, 1.0);
	glVertex3f(indent, 1.0, 0); // v2 Right-Centre Top
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, -1.0, 0); // v3 Right Bottom
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void DrawTexturedSquare(vector<Vector3f> vertices)
{
	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
				 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, ColouredRectanglesTexture);
	glTexParameterf(GL_TEXTURE_2D,
					GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D,
					GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D,
					GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D,
					GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// Draw a rectangle with a texture mapped onto it
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(vertices[0].X, vertices[0].Y, vertices[0].Z); // v0 Left Bottom
	glTexCoord2f(0.0, 1.0);
	glVertex3f(vertices[1].X, vertices[1].Y, vertices[1].Z); // v1 Left Top
	glTexCoord2f(1.0, 1.0);
	glVertex3f(vertices[2].X, vertices[2].Y, vertices[2].Z); // v2 Right Top
	glTexCoord2f(1.0, 0.0);
	glVertex3f(vertices[3].X, vertices[3].Y, vertices[3].Z); // v3 Right Bottom
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void DrawTexturedCube()
{
	vector<Vector3f> vertices = {Vector3f(-1.0, -1.0, -1.0), Vector3f(-1.0, 1.0, -1.0), Vector3f(1.0, 1.0, -1.0), Vector3f(1.0, -1.0, -1.0)};
	DrawTexturedSquare(vertices);

	vertices = {Vector3f(-1.0, -1.0, 1.0), Vector3f(-1.0, 1.0, 1.0), Vector3f(1.0, 1.0, 1.0), Vector3f(1.0, -1.0, 1.0)};
	DrawTexturedSquare(vertices);

	vertices = {Vector3f(-1.0, -1.0, 1.0), Vector3f(-1.0, -1.0, -1.0), Vector3f(1.0, -1.0, -1.0), Vector3f(1.0, -1.0, 1.0)};
	DrawTexturedSquare(vertices);

	vertices = {Vector3f(-1.0, 1.0, 1.0), Vector3f(-1.0, 1.0, -1.0), Vector3f(1.0, 1.0, -1.0), Vector3f(1.0, 1.0, 1.0)};
	DrawTexturedSquare(vertices);

	vertices = {Vector3f(1.0, 1.0, 1.0), Vector3f(1.0, 1.0, -1.0), Vector3f(1.0, -1.0, -1.0), Vector3f(1.0, -1.0, 1.0)};
	DrawTexturedSquare(vertices);

	vertices = {Vector3f(-1.0, 1.0, 1.0), Vector3f(-1.0, 1.0, -1.0), Vector3f(-1.0, -1.0, -1.0), Vector3f(-1.0, -1.0, 1.0)};
	DrawTexturedSquare(vertices);
}

typedef struct materialStruct
{
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess[4];
} Material;

Material redPlasticMaterial = {
	{0.4, 0.0, 0.0, 1.0}, // Ambient
	{0.9, 0.0, 0.0, 1.0}, // Diffuse
	{0.8, 0.8, 0.8, 1.0}, // Specular
	32					  // Shininess
};

Material greenPlasticMaterial = {
	{0.0, 0.4, 0.0, 1.0}, // Ambient
	{0.0, 0.9, 0.0, 1.0}, // Diffuse
	{0.8, 0.8, 0.8, 1.0}, // Specular
	8					  // Shininess
};

void SetMaterial(Material *m)
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, m->ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m->diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, m->shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m->specular);
}

void SetupLight()
{
	glShadeModel(GL_SMOOTH);
	GLfloat LightAmbient[] = {0.5, 0.5, 0.5, 1};
	GLfloat LightDiffuse[] = {0.5, 0.5, 0.5, 1};
	GLfloat LightSpecular[] = {0.5, 0.5, 0.5, 1};
	GLfloat LightPosition[] = {10, 10, 10, 0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT0);
}

Square *sq = new Square;

static PIXELFORMATDESCRIPTOR pfd{
	sizeof(PIXELFORMATDESCRIPTOR),
	1,						 // Version Number
	PFD_DRAW_TO_WINDOW |	 // Format Must Support Window
		PFD_SUPPORT_OPENGL | // Format Must Support OpenGL
		PFD_DOUBLEBUFFER,	// Must Support Double Buffering
	PFD_TYPE_RGBA,			 // Request An RGBA Format
	16,						 // Select Our Color Depth = 16
	0, 0, 0, 0, 0, 0,		 // Color Bits Ignored
	0,						 // No Alpha Buffer
	0,						 // Shift Bit Ignored
	0,						 // No Accumulation Buffer
	0, 0, 0, 0,				 // Accumulation Bits Ignored
	32,						 // 32 Bit Z-Buffer (Depth Buffer)
	0,						 // No Stencil Buffer
	0,						 // No Auxiliary Buffer
	PFD_MAIN_PLANE,			 // Main Drawing Layer
	0,						 // Reserved
	0, 0, 0					 // Layer Masks Ignored
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_IGRALAB, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_IGRALAB));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			DrawGLScene();
			SwapBuffers(hDC);
		}
	}

	return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IGRALAB));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(NULL);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	/*HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);*/
	//Changed for IGRA
	hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
						 760, 340, width, height, nullptr, nullptr, hInstance, nullptr);

	InitOpenGL();

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	//Starting a timer
	SetTimer(hWnd, NULL, 0, NULL);
	ReSizeGLScene(width, height);

	return TRUE;
}
float GlobalRot = 0;
//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:

		switch (wmId)
		{
		case ID_SHAPE_CIRCLE:
			CheckMenuItem(GetMenu(hWnd), ID_SHAPE_CIRCLE, MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd), ID_SHAPE_SQURE, MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd), ID_SHAPE_TRIANGLE, MF_UNCHECKED);
			break;

		case ID_SHAPE_SQURE:
			CheckMenuItem(GetMenu(hWnd), ID_SHAPE_CIRCLE, MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd), ID_SHAPE_SQURE, MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd), ID_SHAPE_TRIANGLE, MF_UNCHECKED);
			break;

		case ID_SHAPE_TRIANGLE:
			CheckMenuItem(GetMenu(hWnd), ID_SHAPE_CIRCLE, MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd), ID_SHAPE_SQURE, MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd), ID_SHAPE_TRIANGLE, MF_CHECKED);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;

	// Added for IGRA
	case WM_SIZE:
	{
		ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));
		break;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);

	case WM_LBUTTONDOWN:
	{
		// Get the left mouse press coordinates
		int mouseX = LOWORD(lParam);
		int mouseY = HIWORD(lParam);

		// Adjust the circle coordinates
		//display mouse coordinate in a dialog box
		//display mouse coordinate in a MessageBox
		//convert mouse click coordinate to opengl coordinate
		convertWindowToOpenGLCoordinates(mouseX, mouseY, xPosShape, yPosShape);

		sq->HandleLButtonDown(xPosShape, yPosShape);

		//char message[255];
		//sprintf_s(message, "x= %d, y=%d", mouseX, mouseY);
		//MessageBoxA(NULL, message, "Mouse Press Event"
		//	, MB_OK | MB_ICONINFORMATION);

		break;
	}

	case WM_MOUSEMOVE:
	{
		int mouseX = LOWORD(lParam);
		int mouseY = HIWORD(lParam);

		convertWindowToOpenGLCoordinates(mouseX, mouseY, xPosShape, yPosShape);

		// Only do this if mouse left button is currently pressed
		if (wParam && MK_MBUTTON && sq->isSelected)
		{
			sq->HandleMouseMove(xPosShape, yPosShape);
		}
		break;
	}

	case WM_TIMER:
	{
		//Handle Timer Events
		GlobalRot += 1;
		break;
	}
	}
	return 0;
}

void ReSizeGLScene(GLsizei width, GLsizei height)
{
	if (height == 0) // Prevent A Divide By Zero By
		height = 1;  // Making Height Equal One
			// Reset The Current Viewport
	glViewport(0, 0, width, height);
	// Select The Projection Matrix
	glMatrixMode(GL_PROJECTION);
	// Reset The Projection Matrix
	glLoadIdentity();
	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

int InitOpenGL()
{
	CheckMenuItem(GetMenu(hWnd), ID_SHAPE_CIRCLE, MF_UNCHECKED);
	CheckMenuItem(GetMenu(hWnd), ID_SHAPE_SQURE, MF_CHECKED);
	CheckMenuItem(GetMenu(hWnd), ID_SHAPE_TRIANGLE, MF_UNCHECKED);

	// Get Device Dontext
	if (!(hDC = GetDC(hWnd)))
	{
		MessageBox(NULL, L"Can't Create A GL Device Context.",
				   L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	// Check if Windows can find a matching Pixel Format
	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))
	{
		MessageBox(NULL, L"Can't Find A Suitable PixelFormat.",
				   L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	// Try to set pixel format
	if (!SetPixelFormat(hDC, PixelFormat, &pfd))
	{
		MessageBox(NULL, L"Can't Set The PixelFormat.", L"ERROR",
				   MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	// Get a Rendering context
	if (!(hRC = wglCreateContext(hDC)))
	{
		MessageBox(NULL, L"Can't Create A GL Rendering Context.",
				   L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	// Activate Rendering Context
	if (!wglMakeCurrent(hDC, hRC))
	{
		MessageBox(NULL, L"Can't Activate The GL Rendering Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	SetupLight();

	mySpere = gluNewQuadric();
	gluQuadricDrawStyle(mySpere, GLU_FILL);

	myCylinder = gluNewQuadric();
	gluQuadricDrawStyle(myCylinder, GLU_FILL);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	CreateCheckerBoardTexture();
	CreateColouredRectanglesTexture();

	clock = new Clock;
	clock->Start();

	return 1;
}

float cubeVertices[][3] = {
	{-1.0f, -1.0f, 1.0f}, // v0
};

int cubeIndices[] = {
	0, 1, 2, 3, // Front Face
	7, 6, 5, 4, // Back Face
	2, 6, 7, 3, // Right Face
	0, 4, 5, 1, // Left Face
	1, 5, 6, 2, // Top Face
	3, 7, 4, 0  // Bottom Face
};

float cubeColors[][3] = {
	0, 1, 0,	  // Front is Green
	1, 0, 0,	  // Back is Red
	0, 0, 1,	  // Right is Blue
	1, 1, 0,	  // Left is Yellow
	0, 0, 0,	  // Top is Black
	0.5, 0.5, 0.5 // Bottom is Grey
};

vector<float> test = vector<float>();

int currentSegmentFrame = 0;
double currentSegmentTime = 0;
#include <string>

enum class RotationState
{
	ACCELERATING = 20,
	DECELERATION = 40,
	MAXVELOCITY = 360,
	STANDSTIL = 0
};
RotationState stateRot = RotationState::ACCELERATING;
void DrawGLScene()
{
	double deltatime = clock->DeltaTime();
	int totaltime = (int)clock->TimePassedSinceStartTime() % ((int)(((double)1 / (double)RotationState::ACCELERATING) * 380) + 5 + (int)(((double)1 / (double)RotationState::DECELERATION) * 380) + 5);
	currentSegmentFrame++;
	currentSegmentTime += deltatime;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1, 1, 1, 1); // White
							  // Locate camera in the (1,1,1) position then look at
							  // origin (the intersection of the axis system)
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float speed = 1;
	float distance = 15;

	float relativeX = sin(xPosShape * speed) * distance;
	float relativeY = sin(yPosShape * speed) * distance;
	float relativeZ = cos(xPosShape * speed) * cos(yPosShape * speed) * distance;

	gluLookAt(relativeX, relativeY, relativeZ, // Camera's position
			  0, 0, 0,						   // Camera's target to look at
			  0, 1, 0);						   // Orientation of camera
	Draw3D_AxisSystem();

	string statestr = "";

	switch (totaltime)
	{
	case 0:
		stateRot = RotationState::ACCELERATING;
		break;

	case (int)((((double)1 / (double)RotationState::ACCELERATING)) * 380):
		stateRot = RotationState::MAXVELOCITY;
		break;

	case (int)((((double)1 / (double)RotationState::ACCELERATING)) * 380) + 5:
		stateRot = RotationState::DECELERATION;
		break;

	case (int)((((double)1 / (double)RotationState::DECELERATION)) * 380) + 5 + (int)(((double)1 / (double)RotationState::ACCELERATING) * 380):
		stateRot = RotationState::STANDSTIL;
		break;
	}

	switch (stateRot)
	{
	case RotationState::ACCELERATING:
		//Accelerate
		statestr = "ACCELERATING";
		rotationalVelicityInDegreesPerSeconds = rotationalVelicityInDegreesPerSeconds + ((double)RotationState::ACCELERATING) * deltatime >= ((double)RotationState::MAXVELOCITY) ? ((double)RotationState::MAXVELOCITY) : rotationalVelicityInDegreesPerSeconds + ((double)RotationState::ACCELERATING) * deltatime;
		yRot += deltatime * rotationalVelicityInDegreesPerSeconds;
		break;

	case RotationState::DECELERATION:
		statestr = "DECELERATION";
		rotationalVelicityInDegreesPerSeconds = rotationalVelicityInDegreesPerSeconds - ((double)RotationState::DECELERATION) * deltatime <= ((double)RotationState::STANDSTIL) ? ((double)RotationState::STANDSTIL) : rotationalVelicityInDegreesPerSeconds - ((double)RotationState::DECELERATION) * deltatime;
		yRot += deltatime * rotationalVelicityInDegreesPerSeconds;
		break;

	case RotationState::MAXVELOCITY:
		statestr = "MAXVELOCITY";
		yRot += deltatime * ((double)RotationState::MAXVELOCITY);
		break;

	case RotationState::STANDSTIL:
		statestr = "STANDSTIL";
		yRot += deltatime * ((double)RotationState::STANDSTIL);
		break;
	}

	if (currentSegmentTime >= 0.2)
	{
		double fps = round(((double)currentSegmentFrame / currentSegmentTime) * 100) / 100;
		string text = "FPS: " + to_string(fps).substr(0, 5) + " | State: " + statestr + " - " + to_string(rotationalVelicityInDegreesPerSeconds);
		SetWindowTextA(hWnd, text.c_str());
		currentSegmentFrame = 0;
		currentSegmentTime = 0;
	}

	/*
	glPushMatrix();
	glRotatef(yrot, 0, 1, 0);
	yrot += 5;
	DrawCube();
	glPopMatrix();
	*/

	/*
	glPushMatrix();
	glTranslatef(3, 0, 0);
	DrawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3, 0, 0);
	DrawCube();
	glPopMatrix();
	*/
	//Make the transformatiob private

	// Local Rotation can be done by first doing a rotation followed by a translation
	//Alert: the order of transformation is to be read from botton to top
	//glRotatef(yrot, 0, 1, 0);// Second transformation
	//glTranslatef(0, 0, 5); // First transformation
	//yrot += 1;
	//glScalef(5, 0.1f, 5);
	//DrawCube();

	//sq->Draw();

	//DrawCircle(0.1, xPosCircle, yPosCircle, 32);

	//DrawSineCurves(-1, 0, 5.f * xPosShape, yPosShape);

	//DrawBlockySpiral(1000);

	//DrawPyramid(GL_TRIANGLES);
	//DrawPyramid(GL_LINE_LOOP);
	//DrawPyramid();

	//DrawSphere(1);

	//DrawDumbbell();

	//DrawSpheresonAxis(2.5, 10);

	//DrawPyramidCircle(32, 4, 0.2);
	
	DrawSurrealSphere(8);

	/*
	double timePassedAbsInSeconds = GetTimePassedSinceStart();
	glPushMatrix();
	yRot = (timePassedAbsInSeconds)*
		rotationalVelicityInDegreesPerSeconds;
	glRotatef(yRot, 0, 1, 0);
	DrawTexturedCube();
	glPopMatrix();
	*/

	glPushMatrix();
	glRotatef(yRot, 0, 1, 0);
	DrawTexturedCube();
	glPopMatrix();

	//DrawTexturedTriangle();

	glLoadIdentity();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Draw2D_AxisSystem()
{
	glBegin(GL_LINE_STRIP);
	glColor3f(0, 1, 0);
	glVertex2f(0, 1);
	glColor3f(0, 1, 0);
	glVertex2f(0, 0);
	glColor3f(1, 0, 0);
	glVertex2f(0, 0);
	glColor3f(1, 0, 0);
	glVertex2f(1, 0);
	glEnd();
}

void Draw3D_AxisSystem()
{
	glBegin(GL_LINES);
	// Draw positive x-axis as red
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(1000, 0, 0);
	// Draw positive y-axis as green
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1000, 0);
	// Draw positive z-axis as blue
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1000);
	glEnd();
}

void DrawPyramid()
{
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);				  // Vertices in clockwise order
	glPolygonMode(GL_FRONT, GL_FILL); // Solid cube
									  // Front Face in green
									  // Vertices in clock wise order v0, v1, v2, v3
									  //Back

	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0.0, 0);		  //Red
	glVertex3f(1.0, -1.0, -1);	// v7
	glVertex3f(0, 1.0, 0);		  // v6
	glVertex3f(-1.0, -1.0, -1.0); // v4
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0.0, 0);   //Red
	glVertex3f(-1.0, 3, -1);  // v7
	glVertex3f(0, 1.0, 0);	// v6
	glVertex3f(1.0, 3, -1.0); // v4
	glEnd();
	//Bottom
	glBegin(GL_POLYGON);
	glColor3f(0.5, 0.5, 0.5);	//Grey
	glVertex3f(1.0, -1.0, 1.0);  // v3
	glVertex3f(1.0, -1.0, -1.0); // v7
	glVertex3f(-1.0, -1.0, -1);  // v4
	glVertex3f(-1.0, -1.0, 1);   // v0
	glEnd();
	//Front
	glBegin(GL_TRIANGLES);	 // 0, 1, 2, 3
	glColor3f(0.0, 1.0, 0);	//green
	glVertex3f(-1.0, -1.0, 1); // v0 Left Bottom
	glVertex3f(0, 1.0, 0);	 // v2 Right Top
	glVertex3f(1.0, -1.0, 1);  // v3 Right Bottom
	glEnd();
	//invert Front
	glBegin(GL_TRIANGLES);  // 0, 1, 2, 3
	glColor3f(0.0, 1.0, 0); //green
	glVertex3f(1, 3, 1);	// v0 Left Bottom
	glVertex3f(0, 1.0, 0);  // v2 Right Top
	glVertex3f(-1, 3, 1);   // v3 Right Bottom
	glEnd();
	//Top
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 0);	//black
	glVertex3f(-1.0, 3.0, 1);  // v1
	glVertex3f(-1.0, 3, -1.0); // v5
	glVertex3f(1.0, 3, -1.0);  // v6
	glVertex3f(1.0, 3, 1);	 // v2
	glEnd();
	//Right
	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 0.0, 1);		 //Blue
	glVertex3f(1.0, -1.0, 1);	// v3
	glVertex3f(0, 1.0, 0);		 // v6
	glVertex3f(1.0, -1.0, -1.0); // v7
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 0.0, 1);	//Blue
	glVertex3f(1.0, 3.0, -1);  // v3
	glVertex3f(0, 1.0, 0);	 // v6
	glVertex3f(1.0, 3.0, 1.0); // v7
	glEnd();
	//Left
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 1.0, 0);		//Yellow
	glVertex3f(-1.0, -1.0, 1);  // v0
	glVertex3f(-1.0, -1.0, -1); // v4
	glVertex3f(0, 1.0, 0);		// v6
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 1.0, 0);	//Yellow
	glVertex3f(-1.0, 3.0, -1); // v0
	glVertex3f(-1.0, 3.0, 1);  // v4
	glVertex3f(0, 1.0, 0);	 // v6
	glEnd();
}

void DrawNormal(float xN, float yN, float zN, float sX, float sY, float sZ)
{
	glBegin(GL_LINES);
	glVertex3f(sX, sY, sZ);
	glVertex3f(xN, yN, zN);
	glEnd();
}

void DrawPyramid(int config)
{
	GLfloat Lightgrey[] = {0.8, 0.8, 0.8, 0};
	Vector3f normal;
	Vector3f surface;
	Vector3f linenormal;
	vector<Vector3f> vertices;

	//glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW); // Vertices in clockwise order

	if (config == GL_TRIANGLES)
	{
		glEnable(GL_LIGHTING);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Lightgrey);
	}
	else
	{
		glLineWidth(2.0f);
		glColor3f(0, 0, 0);
	}
	//Back
	vertices = {Vector3f(-1.0, -1.0, -1.0), Vector3f(0, 1.0, 0), Vector3f(1.0, -1.0, -1)};
	normal = Vector3f::CalculateNormal(vertices[0], vertices[1], vertices[2]);
	normal.Normalise();
	surface = Vector3f::CalculateCenter(vertices[0], vertices[1], vertices[2]);
	linenormal = surface + normal;
	DrawNormal(linenormal.X, linenormal.Y, linenormal.Z, surface.X, surface.Y, surface.Z);
	glBegin(config);
	glNormal3f(normal.X, normal.Y, normal.Z);
	for (size_t i = 0; i < 3; i++)
	{
		glVertex3f(vertices[i].X, vertices[i].Y, vertices[i].Z);
	}
	glEnd();

	//Right
	vertices = {Vector3f(1.0, -1.0, -1.0), Vector3f(0, 1.0, 0), Vector3f(1.0, -1.0, 1)};
	normal = Vector3f::CalculateNormal(vertices[0], vertices[1], vertices[2]);
	normal.Normalise();
	surface = Vector3f::CalculateCenter(vertices[0], vertices[1], vertices[2]);
	linenormal = surface + normal;
	DrawNormal(linenormal.X, linenormal.Y, linenormal.Z, surface.X, surface.Y, surface.Z);
	glBegin(config);
	glNormal3f(normal.X, normal.Y, normal.Z);
	for (size_t i = 0; i < 3; i++)
	{
		glVertex3f(vertices[i].X, vertices[i].Y, vertices[i].Z);
	}
	glEnd();

	//Bottom
	vertices = {Vector3f(1.0, -1.0, -1.0), Vector3f(1.0, -1.0, 1.0), Vector3f(-1.0, -1.0, -1)};
	normal = Vector3f::CalculateNormal(vertices[0], vertices[1], vertices[2]);
	normal.Normalise();
	surface = Vector3f::CalculateCenter(vertices[0], vertices[1], vertices[2]);
	linenormal = surface + normal;
	DrawNormal(linenormal.X, linenormal.Y, linenormal.Z, surface.X, surface.Y, surface.Z);
	glBegin(config);
	glNormal3f(normal.X, normal.Y, normal.Z);
	for (size_t i = 0; i < 3; i++)
	{
		glVertex3f(vertices[i].X, vertices[i].Y, vertices[i].Z);
	}
	glEnd();

	vertices = {Vector3f(-1.0, -1.0, 1), Vector3f(-1.0, -1.0, -1.0), Vector3f(1.0, -1.0, 1.0)};
	normal = Vector3f::CalculateNormal(vertices[0], vertices[1], vertices[2]);
	normal.Normalise();
	surface = Vector3f::CalculateCenter(vertices[0], vertices[1], vertices[2]);
	linenormal = surface + normal;
	DrawNormal(linenormal.X, linenormal.Y, linenormal.Z, surface.X, surface.Y, surface.Z);
	glBegin(config);
	glNormal3f(normal.X, normal.Y, normal.Z);
	for (size_t i = 0; i < 3; i++)
	{
		glVertex3f(vertices[i].X, vertices[i].Y, vertices[i].Z);
	}
	glEnd();

	//Left
	vertices = {Vector3f(-1.0, -1.0, -1), Vector3f(-1.0, -1.0, 1), Vector3f(0, 1.0, 0)};
	normal = Vector3f::CalculateNormal(vertices[0], vertices[1], vertices[2]);
	normal.Normalise();
	surface = Vector3f::CalculateCenter(vertices[0], vertices[1], vertices[2]);
	linenormal = surface + normal;
	DrawNormal(linenormal.X, linenormal.Y, linenormal.Z, surface.X, surface.Y, surface.Z);
	glBegin(config);
	glNormal3f(normal.X, normal.Y, normal.Z);
	for (size_t i = 0; i < 3; i++)
	{
		glVertex3f(vertices[i].X, vertices[i].Y, vertices[i].Z);
	}
	glEnd();

	//Front
	vertices = {Vector3f(0, 1.0, 0), Vector3f(-1.0, -1.0, 1), Vector3f(1.0, -1.0, 1)};
	normal = Vector3f::CalculateNormal(vertices[0], vertices[1], vertices[2]);
	normal.Normalise();
	surface = Vector3f::CalculateCenter(vertices[0], vertices[1], vertices[2]);
	linenormal = surface + normal;
	DrawNormal(linenormal.X, linenormal.Y, linenormal.Z, surface.X, surface.Y, surface.Z);
	glBegin(config);
	glNormal3f(normal.X, normal.Y, normal.Z);
	for (size_t i = 0; i < 3; i++)
	{
		glVertex3f(vertices[i].X, vertices[i].Y, vertices[i].Z);
	}
	glEnd();

	if (config == GL_TRIANGLES)
	{
		glDisable(GL_LIGHTING);
	}
}

void DrawBlockySpiral(int nrOfLines)
{
	glBegin(GL_LINE_STRIP);
	glColor3f(0, 0, 0);

	int currentline = 0;
	float increment = 0.01f;
	float x = 0;
	float y = 0;
	int directtion = -1;
	glVertex2f(x, y);
	x = increment;
	glVertex2f(x, y);
	y = increment;
	glVertex2f(x, y);
	do
	{
		glColor3f(directtion * x, x, y);
		for (size_t i = 0; i < currentline + 2; i++)
		{
			x = x + increment * directtion;
			glVertex2f(x, y);
		}
		glColor3f(y, x, directtion * y);
		for (size_t i = 0; i < currentline + 2; i++)
		{
			y = y + increment * directtion;
			glVertex2f(x, y);
		}
		directtion = directtion * -1;
		currentline++;
	} while (currentline <= nrOfLines && (x <= 1.0f && y <= 1.0f));

	glEnd();
}

float degToRad(float degAngle)
{
	double pi = 3.1415926535; // You could be more precise!
	return degAngle / 180.0 * pi;
}

void DrawCircle(float r, float xc, float yc, int nrOfPoints)
{
	float angularStep = 360.0 / nrOfPoints;
	float x, y = 0;		// To store coordinates of circle vertices
	glColor3f(0, 0, 0); // Our circle will be drawn black
						// Draw the circle with a series of straight lines
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < nrOfPoints; i++)
	{
		//We need to convert from degree to radians!
		x = xc + r * cos(degToRad(angularStep * i));
		y = yc + r * sin(degToRad(angularStep * i));
		glVertex2f(x, y);
	}
	glEnd();
}

void DrawSineCurves(float startX, float startY, float nrOfSineCurves, float scale)
{
	float definition = 100;
	glBegin(GL_LINE_STRIP);
	glColor3f(0, 0, 0);
	for (int i = 0; i < 5 * definition * (abs((int)nrOfSineCurves) == 0 ? 1 : abs((int)nrOfSineCurves)); i++)
	{
		scale = scale <= 0 ? 0 : scale - 0.001f;
		glVertex2f(startX, abs(sin(degToRad((startX + 1) * (180 * nrOfSineCurves))) * scale + startY));
		startX += (0.5f / definition);
	}
	glEnd();
}

void convertWindowToOpenGLCoordinates(int xWinPos, int yWinPos, float &xOpenGLPos, float &yOpenGLPos)
{
	//Normalise pixels
	float normalisexWinPos = xWinPos * 2.0 / width;
	float normaliseyWinPos = (yWinPos + 60) * 2.0 / height;
	xOpenGLPos = normalisexWinPos > 1 ? normalisexWinPos - 1 : 0 - (1 - normalisexWinPos);
	yOpenGLPos = normaliseyWinPos > 1 ? (0 - (1 - normaliseyWinPos)) * -1 : (normaliseyWinPos - 1) * -1;
}

void DrawSphere(float radius)
{
	glEnable(GL_LIGHTING);
	gluQuadricNormals(mySpere, GLU_SMOOTH);
	SetMaterial(&redPlasticMaterial);
	gluSphere(mySpere, radius, 64, 64);
	glDisable(GL_LIGHTING);
}

void DrawCylinder()
{
	glEnable(GL_LIGHTING);
	gluQuadricNormals(myCylinder, GLU_SMOOTH);
	SetMaterial(&greenPlasticMaterial);
	gluCylinder(myCylinder, 1, 1, 10, 64, 64);
	glDisable(GL_LIGHTING);
}

void DrawDumbbell()
{
	glPushMatrix();
	glTranslatef(-5, 0, 0);
	DrawSphere(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5, 0, 0);
	DrawSphere(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5, 0, 0);
	glRotatef(90, 0, 1, 0);
	DrawCylinder();
	glPopMatrix();
}

void DrawSpheresonAxis(float startsize, int iterations)
{
	float size = startsize;
	DrawSphere(size);
	for (size_t i = 0; i < iterations; i++)
	{
		size = size / 2;
		float distance = (startsize + startsize / 2.0) * 2 - ((startsize + startsize / 2.0) / (pow(2, (i / 1))));
		for (size_t j = 0; j < 3; j++)
		{
			glPushMatrix();
			glTranslatef(j == 0 ? (1 * distance) : 0, j == 1 ? (1 * distance) : 0, j == 2 ? (1 * distance) : 0);
			DrawSphere(size);
			glPopMatrix();
		}
	}
}

void DrawPyramidCircle(int count, float distance, float scale)
{
	float step = 360.0f / count;
	for (size_t i = 0; i < count; i++)
	{
		glPushMatrix();
		glRotatef(step * i, 0, 1, 0);
		glTranslatef(distance, 0, 0);
		glRotatef(GlobalRot * 4, 0, 1, 0);
		glScalef(scale, scale, scale);
		DrawPyramid();
		glPopMatrix();
	}
}

void DrawSurrealSphere(int layers)
{
	float direction = 1;
	for (size_t i = 0; i < layers; i++)
	{
		glRotatef((360.0f / layers) * i, 1, 0, 0);
		glPushMatrix();
		glRotatef(yRot * direction / 2, 1, 1, 0);
		DrawPyramidCircle(64, 6, 0.2f);
		glPopMatrix();
		direction = -direction;
		glRotatef((360.0f / layers) * i, 0, 1, 0);
		glPushMatrix();
		glRotatef(yRot * direction / 2, 0, 1, 1);
		DrawPyramidCircle(64, 6, 0.2f);
		glPopMatrix();
		direction = -direction;
		glRotatef((360.0f / layers) * i, 0, 0, 1);
		glPushMatrix();
		glRotatef(yRot * direction / 2, 1, 0, 1);
		DrawPyramidCircle(64, 6, 0.2f);
		glPopMatrix();
		direction = -direction;
		glRotatef((360.0f / layers) * i, 1, 0, 0);
		glPushMatrix();
		glRotatef(yRot * direction / 2, 1, 1, 1);
		DrawPyramidCircle(64, 6, 0.2f);
		glPopMatrix();
		direction = -direction;
		glRotatef((360.0f / layers) * i, 0, 0, 1);
		glPushMatrix();
		glRotatef(yRot * direction / 2, 1, 1, 1);
		DrawPyramidCircle(64, 6, 0.2f);
		glPopMatrix();
		direction = -direction;
		glScalef(1 - 0.2 / (i + 1), 1 - 0.2 / (i + 1), 1 - 0.2 / (i + 1));
	}
}
