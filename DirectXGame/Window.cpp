#include "Window.h"
#include <tchar.h>

Window* window = nullptr;

Window::Window()
{
}

Window::~Window()
{
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
		case WM_CREATE:
		{
			//Fired when Window is created
			window->setHWND(hwnd);
			window->onCreate();
			break;
		}
		case WM_DESTROY:
		{
			//Fired when window is to be destroyed
			window->onDestroy();
			::PostQuitMessage(0);
			break;
		}
		default:
			return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}

	return NULL;
}

bool Window::init()
{
	//Setting up WNDCLASSEX object
	WNDCLASSEX wc;
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = L"MyWindowClass";
	wc.lpszMenuName = L"";
	wc.style = NULL;
	wc.lpfnWndProc = &WndProc;

	if (!::RegisterClassEx(&wc)) //If the registration of this class fails, this function will return false
		return false;

	if (!window)
		window = this;

	m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MyWindowClass", L"DirectX Application", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, NULL, NULL, NULL, this);

	if (!m_hwnd) //If the creation fails return false
		return false;

	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);

	//set this flag to indicate the window is initalized and running
	m_is_run = true;
	return true;
}

bool Window::broadcast()
{
	MSG msg;

	//On Update must be called BEFORE the processing of messages, otherwise when we destroy our window, we would release all the resources, and on update would be called with said resources
	window->onUpdate(); //Since this will be in main, we can call onUpdate here to render all our graphics

	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}


	Sleep(0); //Small pause so these messages can be handled by the CPU easier
	return true;
}

bool Window::release()
{
	if (::DestroyWindow(m_hwnd))
		//Failed to destroy the window
		return false;


	return true;
}

bool Window::isRun()
{
	return m_is_run;
}

RECT Window::getClientWindowRect()
{
	RECT rc;
	::GetClientRect(this->m_hwnd, &rc);
	return rc;
}

void Window::setHWND(HWND hwnd)
{
	this->m_hwnd = hwnd;
}


void Window::onCreate()
{
}

void Window::onUpdate()
{
}

void Window::onDestroy()
{
	m_is_run = false;
}
