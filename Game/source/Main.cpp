#include <windows.h>
#include "../../Engine/source/Window.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	Baryon::Window window({1600, 900}, Baryon::Window::WINDOWED_UNLOCKED);

	DirectX::XMINT2 size = window.getClientSize();

	// TODO: move main loop to Engine
	MSG msg = {};
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
    return 0;
}