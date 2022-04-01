#pragma once
#include "logging.h"

#include<Windows.h>
#include <clocale>
#include <string>

#include <iostream>
#include <fstream>
#include <conio.h>
#include <stdio.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

/* /DELAYLOAD python*.dll */
#include<Python.h>

//#include "logging.h"

#define MAX_LENGTH 1024

#define CONSOLE_
#undef CONSOLE_

#ifdef CONSOLE_
#else 
#define WINDOWS_
#endif // CONSOLE_

#define PYZIP(V) L"python"#V".zip";
#define PYDLL(V) L"python"#V".dll";

using namespace std;

class PyTub {
private:
	void _InitParameter(wstring root);

public:
	PyTub();
	PyTub(const wchar_t* p);
	~PyTub();

//private:
public:
	bool InitRuntime();
	void AppendPath(wchar_t* p);

public:
	bool AfterAppendPath(wchar_t* p);

	bool RunString(const char* line);
	bool RunString(const wchar_t* line);

	bool RunFile(const char* file);
	bool RunFile(const wchar_t* file);

	bool RunByteFile(const wchar_t* file);
	//template<typename T>
	//bool RunFunc(T t);
	bool InteractiveMode(int argc, wchar_t** argv);
	bool InteractiveMode(int argc, char** argv);
	bool InteractiveMode();

	// using PyRun_SimpleString to simulate Python Console ( Py_Main )
	bool SimpleConsole();
	
public:
	void Print();

public:
	wstring RootPath;
	wstring PackagesPath;
	wstring DLLPath;
	wstring ScriptPath;
	wstring CurrentPath;

	wstring ProgramName;
	wstring EntryName = L"main";

private:
	wstring PYPATH;
	

	wchar_t msg[MAX_LENGTH]={L"\0"};
	wchar_t path[MAX_LENGTH] = { L"\0" };
	wchar_t command[MAX_LENGTH] = { L"\0" };

	wchar_t zip[32] = PYZIP(310);
	wchar_t dll[32]= PYDLL(310);

	wchar_t dllDir[1024] = { L"\0" };
	bool flag = false;

public:
	bool GetFlag();
};

void AttachParentConsole();
void ClearConsole();
void DetachParentConsole();
bool RedirectConsoleIO();
