#include <windows.h>
#include "../../Engine/source/Engine.h"
#include "../../Engine/source/Mesh.h"
#include "../../Engine/source/Input.h"
#include "GameWindow.h"

using namespace Baryon;

Camera camera{70, 1, 0.0001, 1000, {2, 1.5, -5}};
GameWindow* window;

void left(float f)
{
	camera.translate({-0.1f, 0, 0});
}

void right(float f)
{
	camera.translate({0.1f, 0, 0});
}

void up(float f)
{
	camera.translate({0, 0.1f, 0});
}

void down(float f)
{
	camera.translate({0, -0.1f, 0});
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

int* get()
{
	static int i = 0;
	return &i;
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
	Input::bindFunctionToInput(up, Input::KEYBOARD_ARROW_UP);
	Input::bindFunctionToInput(down, Input::KEYBOARD_ARROW_DOWN);

	Input::bindFunctionToInput(changeStyle, Input::KEYBOARD_SPACE);
	Input::bindFunctionToInput(changeRes, Input::KEYBOARD_ENTER);

	window->setActiveCamera(&camera);
	Mesh cube;
	engine.getRenderer().submitMesh(cube);

	engine.run();

	delete window;
	return EXIT_SUCCESS;
}
