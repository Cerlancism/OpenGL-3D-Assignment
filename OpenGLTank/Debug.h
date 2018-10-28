#pragma once
//Debug class for displaying output in the visual studio output environment
class Debug
{
public:
	static void Log(std::string log, std::string prefix =  "DEBUG");
	static void Log(char a[]);
	static void Log(char a[], std::string log);
};

