#include "CrossRelfection.h"


static std::map<std::string, Class*> RelfectionClass;

void SetRelfectionClass(const char *name, Class *pClass)
{
	RelfectionClass[name] = pClass;
}

Class* GetRelfectionClass(const char *name)
{
	std::map<std::string, Class*>::const_iterator itClass = RelfectionClass.find(name);
	return itClass != RelfectionClass.end() ? itClass->second : NULL;
}