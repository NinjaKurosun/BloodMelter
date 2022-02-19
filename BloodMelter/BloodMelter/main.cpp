#include <Windows.h>
#include "BloodMelter.h"

BOOL CALLBACK MonitorEnum(HMONITOR hMon, HDC hdc, LPRECT lprcMonitor, LPARAM pData)
{
	BloodMelter* blood = new BloodMelter((HINSTANCE)pData, *lprcMonitor);
	blood->run();
	return TRUE;
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//BloodMelter blood(hInstance, RECT{0,0, 1000, 100});
	//blood.run();

	EnumDisplayMonitors(0, 0, MonitorEnum, (LPARAM)hInstance);

//--------------------------------------------------------------------------------------------------------------------------
	MSG Msg;
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return 0;
}