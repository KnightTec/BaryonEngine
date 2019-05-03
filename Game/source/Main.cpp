#include <windows.h>
#include "../../Engine/source/GameWindow.h"
#include "../../Engine/source/Engine.h"
#include "../../Engine/source/Mesh.h"
#include "../../Engine/source/Input.h"

using namespace Baryon;

Camera camera{ 70, 1, 0.0001, 1000, {2, 1.5, -5} };

void left(float f)
{
	camera.translate({ -0.05f, 0, 0 });
}
void right(float f)
{
	camera.translate({ 0.05f, 0, 0 });
}
void up(float f)
{
	camera.translate({ 0, 0.05f, 0 });
}
void down(float f)
{
	camera.translate({ 0, -0.05f, 0 });
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	Engine engine;
	if (!engine.initialize())
	{
		return EXIT_FAILURE;
	}
	GameWindow window;
	if (!window.initialize(L"Test Game", {1600, 900}, GameWindow::WINDOWED_SCALABLE))
	{
		return EXIT_FAILURE;
	}

	VirtualScreen screen;
	screen.initialize(window);

	// setup input
	Input::bindFunctionToInput(left, Input::KEYBOARD_ARROW_LEFT);
	Input::bindFunctionToInput(right, Input::KEYBOARD_ARROW_RIGHT);
	Input::bindFunctionToInput(up, Input::KEYBOARD_ARROW_UP);
	Input::bindFunctionToInput(down, Input::KEYBOARD_ARROW_DOWN);

	
	screen.setActiveCamera(&camera);
	Baryon::Mesh cube;
	engine.getRenderer().submitMesh(cube);

	engine.getRenderer().bindVirtualScreen(&screen);

	engine.run();
	return EXIT_SUCCESS;
}
