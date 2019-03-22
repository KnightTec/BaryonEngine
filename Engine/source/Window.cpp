#include "Window.h"

using namespace Baryon;

// TODO: remove this variable
static const bool dpiAware = SetProcessDPIAware();

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static DWORD styles[3] = {
	WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, // windowed
	WS_POPUP, // borderless
	WS_OVERLAPPEDWINDOW // windowed unlocked
};

Window::Window(DirectX::XMINT2 clientSize, STYLE style)
	: windowHandle{nullptr}
{
	WNDCLASS wc = {};
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = L"BaryonWindow";
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(GRAY_BRUSH));
	RegisterClass(&wc);

	HWND hwnd = CreateWindowW(wc.lpszClassName, L"Baryon Game", styles[style], 0, 0, 0, 0, nullptr,
		nullptr, wc.hInstance, nullptr);
	if (hwnd)
	{
		windowHandle = hwnd;
		resize(clientSize);
	}
	else
	{
		MessageBoxW(nullptr, L"Error: Failed to create Window.", L"Baryon Engine", MB_OK | MB_ICONERROR);
		exit(1);
	}
}
DirectX::XMINT2 Window::getClientSize() const
{
	RECT clientRect;
	GetClientRect(windowHandle, &clientRect);
	return { clientRect.right, clientRect.bottom };
}
void Window::setStyle(STYLE newStyle)
{
	SetWindowLong(windowHandle, GWL_STYLE, styles[newStyle]);
	ShowWindow(windowHandle, SW_SHOW);
}
void Window::resize(DirectX::XMINT2 newSize)
{
	DirectX::XMINT2 screenSize = {GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)};

	RECT rect;
	rect.left = 0;
	rect.right = newSize.x;
	rect.top = 0;
	rect.bottom = newSize.y;
	DWORD dwStyle = GetWindowLong(windowHandle, GWL_STYLE);
	AdjustWindowRect(&rect, dwStyle, false);

	// Position window at the center of the screen
	DirectX::XMINT2 position = {(screenSize.x - newSize.x) / 2, (screenSize.y - newSize.y) / 2};

	// If window is larger than the screen, the top left corner will stay on the screen
	if (newSize.x <= screenSize.x && newSize.y <= screenSize.y)
	{
		position.x += rect.left;
		position.y += rect.top;
	}
	if (position.x < 0)
	{
		position.x = rect.left;
	}
	if (position.y < 0)
	{
		position.y = 0;
	}
	SetWindowPos(windowHandle, 0, position.x, position.y, rect.right - rect.left, rect.bottom - rect.top,
	             SWP_SHOWWINDOW | SWP_NOZORDER);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}
