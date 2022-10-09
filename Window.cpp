#include "Window.h"
#include <exception>
#include <iostream>
LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
	switch (msg)
	{
	case WM_CREATE:
	    {
		//Event fired when the window is created
		//collected here
			break;
		}
	case WM_SIZE:
	{
		//Event fired when the window is resized
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (window) window->OnSize();
		break;
	}
	case WM_SETFOCUS:
	{
		//Event fired when window gets the focus

		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if(window)
		   window->OnFocus();

		break;
	}
	case WM_KILLFOCUS:
	{
		//Event fired when window loses the focus

		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->OnKillFocus();

		break;
	}
	case WM_DESTROY :
		{
		//Event fired when window is destroyed

		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->OnDestroy();
		::PostQuitMessage(0);
		    break;
	    }
	case WM_PSD_FULLPAGERECT:
		{

		std::cout << "full";
		}
		

	default:
		return ::DefWindowProc(hwnd, msg, wparam, lparam);

	}
		
	return NULL;
}

Window::Window()
{
//Setting up WINDCLASSEX object
	
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
	wc.lpszMenuName =L"";
	wc.style = NULL;
	wc.lpfnWndProc = &WndProc;

	if (!::RegisterClassEx(&wc)) //If registration of class fails, the functiion will throw exception
		throw std::exception("Window was not created successfully");


	//creation of the window
	m_hwnd=::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MyWindowClass", L"DirectX Application", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
		                    1024, 768, NULL, NULL, NULL, NULL);


//if creation fails throw exception
	if (!m_hwnd)
		throw std::exception("Window was not created successfully");

	//Show the window
	::ShowWindow(m_hwnd,SW_SHOW);
	::UpdateWindow(m_hwnd);

	//Indicate at initialization that the window is running 
	m_is_run = true;
}



bool Window::broadcast()
{
	MSG message;

	if (!this->m_is_init)
	{
		SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);	
		this->OnCreate();
		this->m_is_init = true;
	}
	while (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&message);
		DispatchMessage(&message);

	}
	 this->OnUpdate();

	Sleep(1);

	return true;
}

bool Window::IsRun()
{
	if(m_is_run)
	broadcast();
	return m_is_run;
}

RECT Window::GetClientWindowRect()
{
	RECT rect;
	::GetClientRect(this->m_hwnd, &rect);
	::ClientToScreen(this->m_hwnd,(LPPOINT)&rect.left);
	::ClientToScreen(this->m_hwnd, (LPPOINT)&rect.right);


	return rect;
}

RECT Window::GetScreenSize()
{
	RECT rect;

	rect.right = GetSystemMetrics(SM_CXSCREEN);
	rect.bottom = GetSystemMetrics(SM_CYSCREEN);

	return rect;
}

void Window::OnCreate()
{
}

void Window::OnUpdate()
{
}
void Window::OnDestroy()
{
	//Indicate that the window is not running after being destroyed
	m_is_run = false;
}
void Window::OnFocus()
{
}
void Window::OnKillFocus()
{
}
void Window::OnSize()
{

}
Window::~Window()
{
}

