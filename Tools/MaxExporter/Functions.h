#pragma once

#include "MaxExporter.h"


extern void WriteLog(const char *szString);
extern char* ReplaceInvalidChar(char *szString);
extern BOOL ParseAnimationParams(const char *szString, char *szAnimName, int *startFrame, int *endFrame, int *fps);
extern DWORD HashValue(const char *szString);
