#include "stdafx.h"

//Logs a string using methods like to_string
void Debug::Log(std::string log, std::string prefix)
{
	log = "[" + prefix + "] " + log + "\n";
	OutputDebugStringA(log.c_str());
}
//Logs a string from a form of char array
void Debug::Log(char a[])
{
	OutputDebugStringA(a);
	OutputDebugStringA("\n");
}

void Debug::Log(char a[], std::string log)
{
	OutputDebugStringA(a);
	OutputDebugStringA(" : ");
	OutputDebugStringA(log.c_str());
	OutputDebugStringA("\n");
}
