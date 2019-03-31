#include <windows.h>
#include "../../Engine/source/GameWindow.h"
#include "../../Engine/source/Engine.h"
#include "../../Engine/source/Mesh.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	Baryon::Engine engine;
	if (!engine.initialize())
	{
		return EXIT_FAILURE;
	}
	Baryon::GameWindow window;
	if (!window.initialize(L"Test Game", {1600, 900}, Baryon::GameWindow::WINDOWED_SCALABLE))
	{
		return EXIT_FAILURE;
	}

	Baryon::VirtualScreen screen;
	screen.initialize(window);
	Baryon::Camera camera{ 70, 1, 0.0001, 1000 };
	screen.setActiveCamera(&camera);
	Baryon::Mesh cube;
	engine.getRenderer().submitMesh(cube);

	engine.getRenderer().bindVirtualScreen(&screen);
	engine.run();
	return EXIT_SUCCESS;
}
