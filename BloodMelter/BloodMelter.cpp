#include "BloodMelter.h"
#include "Rand64.h"
#include <cstdint>
#include <cmath>
#include <map>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define COUNT_BRUSH 6

const int ScreenWidth  = GetSystemMetrics(SM_CXSCREEN);
const int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
const HBRUSH br[COUNT_BRUSH]
{ 
	CreateSolidBrush(0x000000FF), CreateSolidBrush(0x000000F3),
	CreateSolidBrush(0x000000DA), CreateSolidBrush(0x000001C0),
	CreateSolidBrush(0x000000A8), CreateSolidBrush(0x0000008A)
};

HWND CreateMaskWindow(const WNDCLASS& wndClass, const HINSTANCE& hInstance);
LRESULT CALLBACK BlobProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK BloodProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BloodMelter::run(HINSTANCE hInstance)
{
	WNDCLASS wndClass  = { 0, BlobProc,  0,0, hInstance, 0, 0, 0, 0, L"Blobs" };
	WNDCLASS wndClass1 = { 0, BloodProc, 0,0, hInstance, 0, 0, 0, 0, L"Blood" };
	CreateMaskWindow(wndClass, hInstance);
	CreateMaskWindow(wndClass1, hInstance);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HWND CreateMaskWindow(const WNDCLASS &wndClass, const HINSTANCE &hInstance)
{
	if (RegisterClass(&wndClass))
	{
		HWND hWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_APPWINDOW | WS_EX_TOPMOST | WS_EX_TRANSPARENT, wndClass.lpszClassName,
			0, WS_VISIBLE | WS_POPUP, 0, 0, ScreenWidth, ScreenHeight, HWND_DESKTOP, 0, hInstance, 0);
		if (hWnd)
		{
			SetWindowLong(hWnd, GWL_HWNDPARENT, (LONG)GetDesktopWindow());
			SetLayeredWindowAttributes(hWnd, 0x00000000, 255, LWA_COLORKEY);

			return hWnd;
		}
	}
	return 0;
}

HDC fuckThisVarible;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK BlobProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
		case WM_CREATE:
		{
			fuckThisVarible = CreateCompatibleDC(GetDC(hWnd));

			SetTimer(hWnd, 0, 50, 0);
			ShowWindow(hWnd, SW_SHOW);
			break;
		}
		case WM_TIMER:
		{
			const uint8_t count = 4;

			uint16_t posH[count];
			uint16_t posW[count];
			uint8_t size = random(0, 8 /2 ) + random(0, 8 / 2);

			posH[0] = randomFocusedOnZeroPlus(ScreenHeight);
			posW[0] = randomFocusedOnZeroPlus(ScreenWidth);

			posH[1] = randomFocusedOnZeroPlus(ScreenHeight);
			posW[1] = ScreenWidth - randomFocusedOnZeroPlus(ScreenWidth);

			posH[2] = ScreenHeight - randomFocusedOnZeroPlus(ScreenHeight);
			posW[2] = ScreenWidth  - randomFocusedOnZeroPlus(ScreenWidth);

			posH[3] = ScreenHeight - randomFocusedOnZeroPlus(ScreenHeight);
			posW[3] = randomFocusedOnZeroPlus(ScreenWidth);

			

			HDC blobs = GetDC(hWnd);
			for (int i = 0; i < count; ++i)
			{
				RECT rect = { posW[i], posH[i], posW[i] + size, posH[i] + size };
				FillRect(blobs, &rect, br[randomFocusedOnZero(COUNT_BRUSH)]);
			}

			ReleaseDC(hWnd, blobs);
			break;
		}
		case WM_CLOSE:
		{
			return 0;
		}
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

LRESULT CALLBACK BloodProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
		case WM_CREATE:
		{
			SetTimer(hWnd, 0, 1000, 0);
			ShowWindow(hWnd, SW_SHOW);
			break;
		}
		case WM_TIMER:
		{
			const uint8_t sizeBlood = 18;

			static std::map<uint16_t, uint16_t> blood;
			
			HDC bloodDC = GetDC(hWnd);
			std::map<uint16_t, uint16_t>::iterator itr;
			for (itr = blood.begin(); itr != blood.end(); ++itr)
			{
				if (random(0, 6) > 3) 
				{
					RECT melterBlood
					{ 
						itr->first, 0, 
						itr->first + randomFocusedOnCenter(sizeBlood), blood[itr->first] += random(0, sizeBlood * 2)
					};
					HBRUSH brush_br = br[random(0, COUNT_BRUSH / 2)];
					FillRect(bloodDC, &melterBlood, brush_br);
				}
			}
			blood[random(0, ScreenWidth)];

			/* old blood generator
			 
			const uint8_t sizeBigBlood       = 6;
			const uint8_t sizeSmallBlood     = 3;
			const uint8_t sizeVerySmallBlood = 2;

			Random64 random(GetTickCount64());

			uint16_t bigPos    = random(0, ScreenWidth);
			uint16_t bigLength = random(0, ScreenHeight / 2);

			uint16_t smallPos    = random(0, sizeSmallBlood) + bigPos;
			uint16_t smallLength = random(0, bigLength / 1.2 + 1) + bigLength;

			uint16_t verySmallPos    = random(0, sizeVerySmallBlood) + smallPos;
			uint16_t verySmallLength = random(0, smallLength / 1.2 + 1) + smallLength;

			HDC blood = GetDC(hWnd);
			
			RECT bigBlood       = { bigPos,       0, bigPos + sizeBigBlood,             bigLength };
			RECT smallBlood	    = { smallPos,     0, smallPos + sizeSmallBlood,         smallLength };
			RECT verySmallBlood = { verySmallPos, 0, verySmallPos + sizeVerySmallBlood, verySmallLength };

			HBRUSH brush_br = br[random(0, count_br)];
			FillRect(blood, &bigBlood, brush_br);
			FillRect(blood, &smallBlood, brush_br);
			FillRect(blood, &verySmallBlood, brush_br);

			ReleaseDC(hWnd, blood);*/
			break;
		}
		case WM_CLOSE:
		{
			return 0;
		}
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}