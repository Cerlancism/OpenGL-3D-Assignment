/*
	SP IGRA CA 2 Assignment AY 2017/2018
	Team 1 - Chen Yu, Algene
*/

// OpenGLTank.cpp : Defines the entry point for the application.
#include "stdafx.h"
#include "OpenGLTank.h"
#include <time.h>
#include "Environment.h"
// Many set up variables and methods moved to OpenGLTank.h

int InitOpenGL()
{
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

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	srand(time(NULL));
	Texture::InitialiseAllGlobal();

	Clock::GlobalClock.Start();

	PlayerTank = new Tank;
	PlayerTank->BuildTree();

	CheckMenuItem(GetMenu(hWnd), ID_PROJECTILESPEED_MEDIUM, MF_CHECKED);
	return 1;
}
// Menu selector list.
vector<int> projectileSpeedMenuList = { ID_PROJECTILESPEED_SLOW, ID_PROJECTILESPEED_MEDIUM, ID_PROJECTILESPEED_FAST };
// Helper function to uncheck menus.
void UnCheckMenuList(vector<int> menulist)
{
	for (size_t i = 0; i < menulist.size(); i++)
	{
		CheckMenuItem(GetMenu(hWnd), menulist[i], MF_UNCHECKED);
	}
}

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
				case IDM_ABOUT:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					break;
				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
				case ID_PROJECTILESPEED_SLOW: // Slow speed projectile menu selection.
					UnCheckMenuList(projectileSpeedMenuList);
					CheckMenuItem(GetMenu(hWnd), wmId, MF_CHECKED);
					Missile::StartSpeed = Missile::MissileSpeed::SLOW;
					break;
				case ID_PROJECTILESPEED_MEDIUM: // Medium speed projectile menu selection.
					UnCheckMenuList(projectileSpeedMenuList);
					CheckMenuItem(GetMenu(hWnd), wmId, MF_CHECKED);
					Missile::StartSpeed = Missile::MissileSpeed::MEDIUM;
					break;
				case ID_PROJECTILESPEED_FAST: // Fast speed projectile menu selection.
					UnCheckMenuList(projectileSpeedMenuList);
					CheckMenuItem(GetMenu(hWnd), wmId, MF_CHECKED);
					Missile::StartSpeed = Missile::MissileSpeed::FAST;
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
		}
		break;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;

		case WM_LBUTTONDOWN:
		{
			// Get the left mouse press coordinates
			int mouseX = LOWORD(lParam);
			int mouseY = HIWORD(lParam);
		}
		break;

		case WM_MOUSEMOVE:
		{
			int mouseX = LOWORD(lParam);
			int mouseY = HIWORD(lParam);
			convertWindowToOpenGLCoordinates(mouseX, mouseY, xPosObject, yPosObject);
		}
		break;

		case WM_KEYDOWN:
		{// Tank controls.
			PlayerTank->HandleKeyDown(wParam);
		}
		break;

		case WM_KEYUP:
		{// Tank controls.
			PlayerTank->HandleKeyUp(wParam);
		}

		case WM_TIMER:
		{
			// Handle timer events.
		}
		break;

		default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	}
	return 0;
}

void Draw3D_AxisSystem()
{
	glBegin(GL_LINES);
	// Draw positive x-axis as red
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(10000, 0, 0);
	// Draw positive y-axis as green
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 10000, 0);
	// Draw positive z-axis as blue
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 10000);
	glEnd();
}

void convertWindowToOpenGLCoordinates(int xWinPos, int yWinPos, float &xOpenGLPos, float &yOpenGLPos)
{
	// Normalise pixels.
	float normalisexWinPos = xWinPos * 2.0 / width;
	float normaliseyWinPos = (yWinPos + 60) * 2.0 / height;
	xOpenGLPos = normalisexWinPos > 1 ? normalisexWinPos - 1 : 0 - (1 - normalisexWinPos);
	yOpenGLPos = normaliseyWinPos > 1 ? (0 - (1 - normaliseyWinPos)) * -1 : (normaliseyWinPos - 1) * -1;
}

void DrawGLScene()
{
	Clock::GlobalClock.Refresh();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1, 1, 1, 1); // White background.
	glMatrixMode(GL_MODELVIEW); 
	// Camera presets.
	float speed = 2;
	float distance = 20;
	// Camera movements based on player tank and mouse position.
	float relativeX = -sin(xPosObject * speed) * distance + PlayerTank->Position.X;
	float relativeY = sin(yPosObject * speed) * distance + PlayerTank->Position.Y;
	float relativeZ = -cos(xPosObject * speed) * cos(yPosObject * speed) * distance + PlayerTank->Position.Z;
	// Limit camera movement angles to prevent it moving to underground.
	relativeY = relativeY < 3 ? 3 : relativeY;

	gluLookAt
	(
		relativeX,				relativeY,				relativeZ,				// Camera's position
		PlayerTank->Position.X, PlayerTank->Position.Y, PlayerTank->Position.Z,	// Camera's target to look at
		0,						1,						0						// Orientation of camera
	);
	// Draw 3D axis for reference.
	// Draw3D_AxisSystem();
	// Draw the game objects collectively.
	PlayerTank->Draw();
	Environment::Global.Draw();
	Missile::DrawAll();

	glLoadIdentity();
	// Update the tank physics.
	PlayerTank->Update();
	//Debug::Log(to_string(Clock::DeltaTime));
	//Debug::Log("Position: " + PlayerTank->Position.ToString() + "     Direction: " + PlayerTank->TurretDirection.ToString());
}