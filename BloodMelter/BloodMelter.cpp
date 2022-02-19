#include "BloodMelter.h"
#include <iterator>
#include <string>
#include <cmath>
#include <map>

#define GWL_HWNDPARENT (-8)

#define COUNT_BRUSH 6

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Random64 BloodMelter::random = GetTickCount64();
const HBRUSH BloodMelter::brush[COUNT_BRUSH] =
{
	CreateSolidBrush(0x000000FF), CreateSolidBrush(0x000000F3),
	CreateSolidBrush(0x000000DA), CreateSolidBrush(0x000001C0),
	CreateSolidBrush(0x000000A8), CreateSolidBrush(0x0000008A)
};

BloodMelter::BloodMelter(const HINSTANCE& tHInstance, RECT tWindow/*= { 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) }*/ ) 
	: hInstance(tHInstance), window(tWindow)
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BloodMelter::run()
{
	static std::wstring blob = L"Blob";
	static std::wstring blood = L"Blood";
	blob.push_back(L'plus');
	blood.push_back(L'plus');

	WNDCLASS wndClass = { 0, BlobProc,  0,0, hInstance, 0, 0, 0, 0, blob.c_str() };
	WNDCLASS wndClass1 = { 0, BloodProc, 0,0, hInstance, 0, 0, 0, 0, blood.c_str() };
	CreateMaskWindow(wndClass, hInstance, window);
	CreateMaskWindow(wndClass1, hInstance, window);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HWND BloodMelter::CreateMaskWindow(const WNDCLASS& wndClass, const HINSTANCE& tHInstance, const RECT& tWindow)
{
	if (RegisterClass(&wndClass))
	{
		const int posX = tWindow.left;
		const int posY = tWindow.top;

		const int Width = tWindow.right - posX;
		const int Height = tWindow.bottom - posY;

		HWND hWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_APPWINDOW | WS_EX_TOPMOST | WS_EX_TRANSPARENT, wndClass.lpszClassName,
			0, WS_VISIBLE | WS_POPUP, posX, posY, Width, Height, HWND_DESKTOP, 0, tHInstance, 0);
		if (hWnd)
		{
			SetWindowLong(hWnd, GWL_HWNDPARENT, (LONG)GetDesktopWindow());
			SetLayeredWindowAttributes(hWnd, 0x00000000, 255, LWA_COLORKEY);

			return hWnd;
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK BloodMelter::BlobProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	RECT window;
	GetWindowRect(hWnd, &window);

	int Width = window.right - window.left;
	int Height = window.bottom - window.top;

	switch (Msg)
	{
		case WM_TIMER:
		{
			const uint8_t count = 4;

			uint16_t posH[count];
			uint16_t posW[count];
			uint8_t size = random.randomFocusedOnCenter(0, 8);

			const uint8_t stretch = 100;

			posH[0] = random.randomFocusedOnZero(0, Height, stretch);
			posW[0] = random.randomFocusedOnZero(0, Width, stretch);

			posH[1] = random.randomFocusedOnZero(0, Height, stretch);
			posW[1] = Width - random.randomFocusedOnZero(0, Width, stretch);

			posH[2] = Height - random.randomFocusedOnZero(0, Height, stretch);
			posW[2] = Width - random.randomFocusedOnZero(0, Width, stretch);

			posH[3] = Height - random.randomFocusedOnZero(0, Height, stretch);
			posW[3] = random.randomFocusedOnZero(0, Width, stretch);



			HDC blobs = GetDC(hWnd);
			for (int i = 0; i < count; ++i)
			{
				RECT rect = { posW[i], posH[i], posW[i] + size, posH[i] + size };
				FillRect(blobs, &rect, brush[random.randomFocusedOnZero(0, COUNT_BRUSH)]);
			}

			ReleaseDC(hWnd, blobs);
			break;
		}
		case WM_CLOSE:
		{
			return 0;
		}
		case WM_CREATE:
		{
			SetTimer(hWnd, 0, 50, 0);
			ShowWindow(hWnd, SW_SHOW);
			break;
		}
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK BloodMelter::BloodProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	RECT window;
	GetWindowRect(hWnd, &window);

	const int Width = window.right - window.left;
	const int Height = window.bottom - window.top;

	switch (Msg)
	{
		case WM_TIMER:
		{
			HDC bloodDC = GetDC(hWnd);

			static std::map<uint16_t, uint16_t> blood; //the first one stores the X coordinate of the blood, the second - the length of the blood
			uint16_t bloodSize = blood.size();
			const uint8_t sizeBlood = 18;

			static double stretch = 1;
			stretch *= 1.09; //so that the blood is distributed more evenly and evenly over time

			static bool state = true;      // toggle
			state = (state) ? false : true;// ^^^^^^

			uint16_t randWidth = random.randomFocusedOnZero(0, Width, stretch); // create blood
			if (state) blood[randWidth]; else blood[Width - randWidth];        // right or left?

			std::map<uint16_t, uint16_t>::iterator iter;
			for (int i = 0; i < bloodSize; ++i)
			{
				iter = blood.begin();

				uint16_t setIter = random.randomFocusedOnZero(0, bloodSize, stretch);                 // selects blood
				if (state) std::advance(iter, setIter); else std::advance(iter, bloodSize - setIter);// right or left?

				//--------------------------------------------------------------------------------------------------------------------------
				
				RECT melterBlood // creates blood
				{
					iter->first, 0, // upper left point
					iter->first + random.randomFocusedOnCenter(0, sizeBlood), iter->second += random(0, sizeBlood / 2) // lower right point
				};
				HBRUSH brush_br = brush[random(0, COUNT_BRUSH / 2)];
				FillRect(bloodDC, &melterBlood, brush_br);
			}
			break;
		}
		case WM_CLOSE:
		{
			return 0;
		}
		case WM_CREATE:
		{
			SetTimer(hWnd, 0, 1200, 0);
			ShowWindow(hWnd, SW_SHOW);
			break;
		}
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}