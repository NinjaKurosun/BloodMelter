#pragma once
#include <Windows.h>
#include "Rand64.h"

class BloodMelter
{
	static Random64 random;
	HINSTANCE hInstance;
	RECT window;

	static const HBRUSH brush[];

public:
	BloodMelter(const HINSTANCE& tHInstance, RECT tWindow = {0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)});

	void run();

protected:
	static HWND CreateMaskWindow(const WNDCLASS& wndClass, const HINSTANCE& tHInstance, const RECT& tWindow);
	static LRESULT CALLBACK BlobProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK BloodProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
};