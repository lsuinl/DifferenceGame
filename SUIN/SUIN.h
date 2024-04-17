/**

	@file      WinApp.h
	@brief     Let's create a window and wrap the message procedure in a class.
	@author    SKYFISH
**/
#pragma once


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class WinApp
{
public:

	WinApp() = default;
	~WinApp() = default;

	void Initialize(HINSTANCE hInstance);

	void Run();

	void Finalize();

	HINSTANCE GetInstance() const { return m_hInstance; }

	HWND GetWindow() const { return m_hWnd; }

	const int GetWidth() const { return width; }
	const int GetHeight() const { return height; }

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	HWND GetHWnd() const
	{
		return m_hWnd;
	}
private:

	HINSTANCE m_hInstance = { 0 };  // HINSTANCE is a handle to an instance of a module.
	HWND m_hWnd = { 0 };			// HWND is a handle to a window.

	int width = 2400;
	int height = 1200;
};

namespace global
{
	const WinApp& GetWinApp();
}
