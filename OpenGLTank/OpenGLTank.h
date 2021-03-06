#pragma once

#include "resource.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;					 // current instance
WCHAR szTitle[MAX_LOADSTRING];		 // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING]; // the main window class name

									 // Added for IGRA
int width = 1280;
int height = 720;
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
float xPosObject = 0.0;
float yPosObject = 0.0;

Tank* PlayerTank;

int InitOpenGL();
void convertWindowToOpenGLCoordinates(int xWinPos, int yWinPos, float &xOpenGLPos, float &yOpenGLPos);
void DrawGLScene();
void ReSizeGLScene(GLsizei width, GLsizei height);

void SetupLight()
{
	glShadeModel(GL_SMOOTH);
	GLfloat LightAmbient[] = { 0.5, 0.5, 0.5, 1 };
	GLfloat LightDiffuse[] = { 0.5, 0.5, 0.5, 1 };
	GLfloat LightSpecular[] = { 0.5, 0.5, 0.5, 1 };
	GLfloat LightPosition[] = { 10, 10, 10, 0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT0);
}

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
	LoadStringW(hInstance, IDC_OPENGLTANK, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OPENGLTANK));

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

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_OPENGLTANK));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_OPENGLTANK);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

					   /*HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
					   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);*/
					   //Changed for IGRA
	int startlocationX = GetSystemMetrics(SM_CXSCREEN);
	int startlocationY = GetSystemMetrics(SM_CYSCREEN);
	hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		(startlocationX - width) / 2, (startlocationY - height) / 2, width, height, nullptr, nullptr, hInstance, nullptr);

	InitOpenGL();

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	//Starting a timer
	SetTimer(hWnd, NULL, 0 , NULL);
	ReSizeGLScene(width, height);

	return TRUE;
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
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 5000.0f);
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
	}
	return (INT_PTR)FALSE;
}