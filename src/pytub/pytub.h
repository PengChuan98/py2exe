#pragma once

#include "logging.h"

#include<Windows.h>
#include <clocale>
#include <string>

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

/* /DELAYLOAD python*.dll */
#include<Python.h>

//#include "logging.h"

#define MAX_LENGTH 1024

using namespace std;

class PyTub {
public:
	PyTub();
	~PyTub();

//private:
public:
	bool InitRuntime();
	
public:
	//bool RunString();
	//bool RunFile();
	//template<typename T>
	//bool RunFunc(T t);

public:
	wstring RootPath;
	wstring HomePath;
	wstring PackagesPath;
	wstring DLLPath;

	wstring ExePath;
	wstring EntryName = L"main";

private:
	wchar_t msg[MAX_LENGTH];
};
