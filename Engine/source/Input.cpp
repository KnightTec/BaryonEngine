#include "Input.h"

using namespace Baryon;

std::pair<float, std::vector<void(*)(float)>> Input::inputCallbacks[10];

void Input::bindFunctionToInput(void(* function)(float), TYPE type)
{
	inputCallbacks[type].second.push_back(function);
}

void Input::handleOSinput(int virtualKeyCode, float value)
{
	switch (virtualKeyCode)
	{
	case VK_LEFT:
		inputCallbacks[KEYBOARD_ARROW_LEFT].first = value;
		break;
	case VK_RIGHT:
		inputCallbacks[KEYBOARD_ARROW_RIGHT].first = value;
		break;
	case VK_UP:
		inputCallbacks[KEYBOARD_ARROW_UP].first = value;
		break;
	case VK_DOWN:
		inputCallbacks[KEYBOARD_ARROW_DOWN].first = value;
		break;
	}
}

void Input::handleGameInput()
{
	for (int i = 0; i < 10; i++)
	{
		if (inputCallbacks[i].first)
		{
			for (void(*callback)(float) : inputCallbacks[i].second)
			{
				callback(0);
			}
		}
	}
}
