#include "GameWindow.h"
#include "input/Input.h"
#include "Engine.h"
#include "StringId.h"
#include "ResourceManager.h"

#include "CameraScript.h"

#include <windows.h>

#include <string>

using namespace Baryon;

GameWindow* window;

void changeStyle()
{
	static int i = 1;
	window->setStyle(static_cast<GameWindow::STYLE>(i));
	i = (i + 1) % 3;
}
void changeRes()
{
	static int i = 5;
	window->setResolution(VirtualScreen::getSupportedResolutions()[i]);
	i = (i + 1) % VirtualScreen::getSupportedResolutions().size();
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	ShowCursor(0);
	WindowsApplication::initialize(hInstance);
	Engine engine;
	if (!engine.initialize())
	{
		return EXIT_FAILURE;
	}
	window = new GameWindow(L"Test Game", VirtualScreen::getSupportedResolutions()[6]);
	engine.injectVirtualScreen(*window);
	
	// setup input
	Input::Axis& moveForwardAxis = Input::addAxis("MoveForward");
	Input::Axis& moveRightAxis = Input::addAxis("MoveRight");
	Input::Axis& moveUpAxis = Input::addAxis("MoveUp");
	Input::Axis& turnAxis = Input::addAxis("Turn");
	Input::Axis& lookUpAxis = Input::addAxis("LookUp");
	Input::Action& switchResolutionAction = Input::addAction("SwitchRes");
	Input::Action& changeStyleAction = Input::addAction("ChangeStyle");
	Input::Axis& speedAxis = Input::addAxis("SpeedUp");

	moveForwardAxis.setInputBinding(KEYBOARD_INPUT::W, 1);
	moveForwardAxis.setInputBinding(KEYBOARD_INPUT::S, -1);
	moveForwardAxis.setInputBinding(GAMEPAD_INPUT::AXIS_LEFT_Y, -1);

	moveRightAxis.setInputBinding(KEYBOARD_INPUT::D, 1);
	moveRightAxis.setInputBinding(KEYBOARD_INPUT::A, -1);
	moveRightAxis.setInputBinding(GAMEPAD_INPUT::AXIS_LEFT_X, 1);

	moveUpAxis.setInputBinding(KEYBOARD_INPUT::E, 1);
	moveUpAxis.setInputBinding(KEYBOARD_INPUT::Q, -1);

	turnAxis.setInputBinding(MOUSE_INPUT::AXIS_X, 1);
	turnAxis.setInputBinding(GAMEPAD_INPUT::AXIS_RIGHT_X, 1);

	lookUpAxis.setInputBinding(MOUSE_INPUT::AXIS_Y, 1);
	lookUpAxis.setInputBinding(GAMEPAD_INPUT::AXIS_RIGHT_Y, 1);

	switchResolutionAction.setInputBinding(KEYBOARD_INPUT::SPACE);
	switchResolutionAction.setInputBinding(GAMEPAD_INPUT::BUTTON_1);

	changeStyleAction.setInputBinding(KEYBOARD_INPUT::RETURN);
	changeStyleAction.setInputBinding(GAMEPAD_INPUT::BUTTON_3);

	speedAxis.setInputBinding(GAMEPAD_INPUT::RIGHT_TRIGGER, 1);

	Input::bindAction("SwitchRes", Input::Action::PRESSED, changeRes);
	Input::bindAction("ChangeStyle", Input::Action::PRESSED, changeStyle);

	//Input::bindAxis("SpeedUp", setSpeed);

	Mesh::import("../../untitled.obj");
	StringId meshPath("mesh.bass");

	World* world = engine.createWorld();

	Entity* cam = world->addCamera();
	cam->getComponent<Transform>()->position.y = 10;
	cam->getComponent<Transform>()->position.z = -10;
	window->setActiveCamera(cam);

	cam->addComponent<ScriptComponent>();
	ScriptComponent* sc = cam->getComponent<ScriptComponent>();
	sc->script = new CameraScript(*cam);

	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			
			Entity* entity = world->addMesh();
			
			auto* t = entity->getComponent<Transform>();
			t->translate(x * 14, 10, y * 14);
			t->scale(5, 5, 5);
			auto* m = entity->getComponent<MeshComponent>();
			m->mesh = ResourceManager::get().getMesh(meshPath);
		}
	}

	window->setStyle(GameWindow::WINDOWED);
	window->show();

	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		engine.mainLoopIteration();
	}
	engine.terminate();

	delete window;
	return EXIT_SUCCESS;
}
