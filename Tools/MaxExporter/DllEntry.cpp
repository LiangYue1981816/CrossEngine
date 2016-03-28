#include "MaxExporter.h"


int controlsInit = FALSE;
HINSTANCE hInstance = NULL;


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpReserved)
{
	hInstance = hinstDLL;

	if (!controlsInit) {
		controlsInit = TRUE;
		InitCommonControls();
	}

	return TRUE;
}

__declspec( dllexport ) const char* LibDescription()
{
	return LIBRARY_DESC;
}

__declspec( dllexport ) int LibNumberClasses()
{
	return 1;
}

__declspec( dllexport ) ClassDesc* LibClassDesc(int i)
{
	static CMaxExporterClassDesc maxExportDesc;

	switch(i) {
	case  0: return &maxExportDesc;
	default: return 0;
	}
}

__declspec( dllexport ) ULONG LibVersion()
{
	return VERSION_3DSMAX;
}

__declspec( dllexport ) ULONG CanAutoDefer()
{
	return 1;
}
