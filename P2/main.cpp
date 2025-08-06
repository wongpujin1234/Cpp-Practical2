
#include <Windows.h>
#include <gl/GL.h>
#include <cstdlib>
#include <ctime>

#pragma comment (lib, "OpenGL32.lib")

#define WINDOW_TITLE "OpenGL Window"

float angle = 0.0;
float scaleFactor = 1.0;
float scaleDirection = 1.0;
float tx = 0, ty = 0, tspeed = 0.1;			//translate x and y
float r = 1, g = 1, b = 1;					//white
float targetR = 1.0, targetG = 1.0, targetB = 1.0;
float lerpSpeed = 0.05;
int changeColorInterval = 300;
int frameCount = 0;
int qno = 1;
float alpha = 1.0f;
float fadeDirection = -1.0f;  // Fade out first

float radius = 0.5f;      // Radius of circular path
float speed = 0.01f;

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		else if (wParam == VK_RIGHT) {
			tx += tspeed;
		}
		else if (wParam == VK_LEFT) {
			tx -= tspeed;
		}
		else if (wParam == VK_UP) {
			ty += tspeed;
		}
		else if (wParam == VK_DOWN) {
			ty -= tspeed;
		}
		else if (wParam == '1') {
			qno = 1;
		}
		else if (wParam == '2') {
			qno = 2;
		}
		else if (wParam == '3') {
			qno = 3;
		}
		else if (wParam == '4') {
			qno = 4;
		}
		else if (wParam == '5') {
			qno = 5;
		}
		else if (wParam == 'R') {
			r = 1.0;
			g = 0.0;
			b = 0.0;
		}
		else if (wParam == 'G') {
			r = 0.0;
			g = 1.0;
			b = 0.0;
		}
		else if (wParam == 'B') {
			r = 0.0;
			g = 0.0;
			b = 1.0;
		}
		else if (wParam == VK_SPACE) {
			r = 1.0;
			g = 1.0;
			b = 1.0;
			tx = 0;
			ty = 0;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------

void p2Demo() {

	glMatrixMode(GL_MODELVIEW);		//modelview
	glLoadIdentity();				//reset the transformation
	glTranslatef(0.5, -0.5, 0.0);	//tx(0.5)RIGHT,ty(-0.5)DOWN
	glRotatef(90, 0.0, 0.0, 1.0);	//rz(90)anti-clockwise
	glScalef(0.5, 0.5, 0.5);		//sx(

	glBegin(GL_TRIANGLES);
	glVertex2f(-0.5, 0.0);
	glVertex2f(0.0, 0.5);
	glVertex2f(0.5, 0.0);
	glEnd();

	glLoadIdentity();		//reset the transformation
	glScalef(0.5, 0.5, 0.5);
	glBegin(GL_QUADS);
	glVertex2f(-0.5, 0.0);
	glVertex2f(-0.5, 0.5);
	glVertex2f(0.5, 0.5);
	glVertex2f(0.5, 0.0);
	glEnd();
}

void p2Q1() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();
	glTranslatef(tx, ty, 0.0);
	glBegin(GL_QUADS);
	glColor3f(r, g, b);
	glVertex2f(-0.5, 0.0);
	glVertex2f(-0.5, 0.5);
	glVertex2f(0.5, 0.5);
	glVertex2f(0.5, 0.0);
	glEnd();
}
void p2Q2() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// Enable blending for fade effect
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Vibrant color list
	static float vibrantColors[6][3] = {
		{1.0,0.0,0.0},	//red
		{0.0,1.0,0.0},	//green
		{0.0,0.0,1.0},	//blue
		{0.67,0.58,0.0},
		{0.48,0.0,1.0},
		{0.0,1.0,1.0},	//cyan
	};

	// Color transition
	frameCount++;
	if (frameCount % changeColorInterval == 0) {
		int idx = rand() % 6;
		targetR = vibrantColors[idx][0];
		targetG = vibrantColors[idx][1];
		targetB = vibrantColors[idx][2];
	}
	r += (targetR - r) * lerpSpeed;
	g += (targetG - g) * lerpSpeed;
	b += (targetB - b) * lerpSpeed;

	// Fade In/Out
	alpha += 0.01f * fadeDirection;
	if (alpha >= 1.0f) {
		alpha = 1.0f;
		fadeDirection = -1.0f;
	}
	else if (alpha <= 0.2f) {
		alpha = 0.2f;
		fadeDirection = 1.0f;
	}

	// Animation - rotate and scale
	angle += 0.5;
	if (angle > 360) angle -= 360;

	scaleFactor += 0.002 * scaleDirection;
	if (scaleFactor > 1.2 || scaleFactor < 0.8) scaleDirection *= -1;

	glLoadIdentity();
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.0);
	glRotatef(angle, 0.0, 0.0, 1.0);
	glScalef(scaleFactor, scaleFactor, 1.0);

	// Use RGBA color
	glColor4f(r, g, b, alpha);

	const int numPoints = 10;
	float radiusOuter = 0.8f;
	float radiusInner = 0.35f;

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0.0f, 0.0f); // center
	for (int i = 0; i <= numPoints; ++i) {
		float angleDeg = i * 36.0f;
		float angleRad = angleDeg * 3.14159f / 180.0f;
		float radius = (i % 2 == 0) ? radiusOuter : radiusInner;
		float x = radius * cosf(angleRad);
		float y = radius * sinf(angleRad);
		glVertex2f(x, y);
	}
	glEnd();

	glPopMatrix();
}

void p2Q3() {

	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Update angle to rotate anti-clockwise
	angle += speed;
	if (angle > 2 * 3.14159f) {
		angle -= 2 * 3.14159f;
	}

	// Calculate new position using angle
	float x = radius * cos(angle);
	float y = radius * sin(angle); // sin(angle) = anti-clockwise movement

	// Draw the moving point
	glPointSize(10.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 0.0f, 0.0f); // Red point
	glVertex2f(x, y);
	glEnd();

}

void p2Q4() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background
	glClear(GL_COLOR_BUFFER_BIT);

	// Update scale factor
	scaleFactor += 0.01f * scaleDirection;
	if (scaleFactor > 1.5f || scaleFactor < 0.5f) {
		scaleDirection *= -1; // Reverse scaling direction
	}

	glLoadIdentity();
	glPushMatrix();

	// Apply scaling
	glScalef(scaleFactor, scaleFactor, 1.0f);

	// Draw a red rectangle (box)
	glColor3f(0.58f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex2f(-0.5f, -0.5f);
	glVertex2f(-0.5f, 0.5f);
	glVertex2f(0.5f, 0.5f);
	glVertex2f(0.5f, -0.5f);
	glEnd();

	glPopMatrix();
}


void display()
{
	switch (qno) {
	case 1:
		p2Demo();
		break;
	case 2:
		p2Q1();
		break;

	case 3:
		p2Q2();
		break;
	case 4:
		p2Q3();
		break;
	case 5:
		p2Q4();
		break;

	}
}
//--------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		900, 10, 300, 300,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	srand(static_cast<unsigned int>(time(NULL)));


	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------