#pragma once
//Debug class for displaying output in the visual studio output environment
class Debug
{
public:
	static void Debug::Log(std::string log);
	static void Debug::Log(char a[]);
	static void Debug::Log(char a[], std::string log);
};

