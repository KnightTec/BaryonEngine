#include <windows.h>

#include "GameWindow.h"

#include "Engine.h"
#include "Mesh.h"
#include "Input.h"


using namespace Baryon;

Camera camera{70, 1, 0.01, 1000, {2, 1.5, -5}};
GameWindow* window;

void left(float f)
{
	camera.translate({-0.1f, 0, 0});
}

void right(float f)
{
	camera.translate({0.1f, 0, 0});
}

void forward(float f)
{
	camera.translate({0, 0, 0.1f});
}

void backward(float f)
{
	camera.translate({0, 0, -0.1f});
}
void turn(float f)
{
	camera.rotate(0, f * 0.01, 0, SPACE::GLOBAL);
}
void lookUpDown(float f)
{
	camera.rotate(f * 0.01, 0, 0);
}

void changeStyle(float)
{
	static int i = 1;
	window->setStyle(static_cast<GameWindow::STYLE>(i));
	i = (i + 1) % 3;
}

void changeRes(float)
{
	static int i = 1;
	window->setResolution(VirtualScreen::getSupportedResolutions()[i]);
	i = (i + 1) % VirtualScreen::getSupportedResolutions().size();
}


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	WindowsApplication::initialize(hInstance);
	Engine engine;
	if (!engine.initialize())
	{
		return EXIT_FAILURE;
	}
	window = new GameWindow(L"Test Game", VirtualScreen::getSupportedResolutions()[0], GameWindow::WINDOWED);

	if (!engine.getRenderer().createVirtualScreen(*window))
	{
		MessageBoxW(nullptr, L"Error: Failed to create VirtualScreen.", L"Baryon Engine", MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}

	// setup input
	Input::bindFunctionToInput(left, Input::KEYBOARD_ARROW_LEFT);
	Input::bindFunctionToInput(right, Input::KEYBOARD_ARROW_RIGHT);
	Input::bindFunctionToInput(forward, Input::KEYBOARD_ARROW_UP);
	Input::bindFunctionToInput(backward, Input::KEYBOARD_ARROW_DOWN);

	Input::bindFunctionToInput(changeStyle, Input::KEYBOARD_SPACE);
	Input::bindFunctionToInput(changeRes, Input::KEYBOARD_ENTER);
	Input::bindFunctionToInput(turn, Input::MOUSE_X);
	Input::bindFunctionToInput(lookUpDown, Input::MOUSE_Y);

	window->setActiveCamera(&camera);
	Mesh cube;
	engine.getRenderer().submitMesh(cube);

	engine.run();

	delete window;
	return EXIT_SUCCESS;
}
