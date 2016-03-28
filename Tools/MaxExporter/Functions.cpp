#include "MaxExporter.h"


//
// 写入日至文件
//
void WriteLog(const char *szString)
{
#ifdef _DEBUG
	FILE *pFile = fopen("C:\\MaxExport.log", "a+");
	fprintf(pFile, "%s", szString);
	fclose(pFile);
#endif
}

//
// 替换字符串中的无效字符
//
char* ReplaceInvalidChar(char *szString)
{
	for (int index = 0; index < (int)strlen(szString); index++) {
		if (szString[index] == ' ' || szString[index] == '#') {
			szString[index] =  '_';
		}
	}

	return szString;
}

//
// 解析动画参数
// 格式 startFrame endFrame fps
//
BOOL ParseAnimationParams(const char *szString, char *szAnimName, int *startFrame, int *endFrame, int *fps)
{
	//
	// 1.  动画名
	//
	int index = 0;

	sprintf(szAnimName, "%s", szString);
	while (strlen(&szAnimName[index]) && szAnimName[index] != ' ') index++;
	szAnimName[index] = 0;

	//
	// 2. 动画参数
	//
	char *szValue = &szAnimName[index + 1];
	int *values[3] = {startFrame, endFrame, fps};

	index = 0;

	do {
		*values[index++] = atoi(szValue);

		do {
			szValue++;
		} while (' ' != *szValue && strlen(szValue));

		if (!strlen(szValue) && index < 3) {
			return FALSE;
		}
	} while (index < 3);

	if (*endFrame < *startFrame || fps <= 0) {
		return FALSE;
	}

	return TRUE;
}

//
// 计算字符串Hash值
//
DWORD HashValue(const char *szString)
{
	DWORD dwHashValue = 0;
	const char *c = szString;

	while (*c) {
		dwHashValue = (dwHashValue << 5) - dwHashValue + (*c == '/' ? '\\' : *c);
		c++;
	}

	return dwHashValue ? dwHashValue : 0xffffffff;
}
