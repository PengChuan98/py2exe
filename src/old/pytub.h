#pragma once
#include "pydll2h.h"

#include <shlwapi.h>
#include<Windows.h>
#include <clocale>
#include <string>

#ifdef _MSC_VER
#pragma comment(lib, "shlwapi.lib")
#endif

class PyTub {
private:
	void _InitParameter(const wchar_t* path);
public:
	PyTub();
	PyTub(const wchar_t* path);
	~PyTub();
	
protected:
	_Py_Initialize Py_Initialize;
	_Py_IsInitialized Py_IsInitialized;
	_Py_Finalize Py_Finalize;

public:
	_Py_SetPythonHome Py_SetPythonHome;
	_Py_SetPath Py_SetPath;

	_Py_GetPath Py_GetPath;
	_Py_GetProgramFullPath Py_GetProgramFullPath;

	_PyRun_SimpleString PyRun_SimpleString;

	//_Py_XDECREF Py_XDECREF;
	//_Py_DECREF Py_DECREF;
	_Py_Dealloc Py_Dealloc;

	_PyErr_Print PyErr_Print;
	_PyErr_Occurred PyErr_Occurred;
	_PyErr_Clear PyErr_Clear;

	_PyObject_Print PyObject_Print;

	_PyUnicode_DecodeFSDefault PyUnicode_DecodeFSDefault;
	_PyImport_Import PyImport_Import;
	_PyObject_GetAttrString PyObject_GetAttrString;
	_PyCallable_Check PyCallable_Check;
	_PyObject_CallObject PyObject_CallObject;

	_Py_BuildValue Py_BuildValue;
	_Py_fopen_obj Py_FOpen;
	_PyRun_SimpleFile PyRun_SimpleFile;


	//_Py_NoneStruct Py_NoneStruct;

	//#define Py_None (&Py_NoneStruct);

	void Py_DECREF(void* obj);
public:
	bool LoadPythonDll();
	void AutoSetPath();
	bool PythonEnvInit();
	void PythonEnvFinish();

	void RunString(const char* command);
	void RunFile(const char* path);

private:
	// There are many bugs in this function 
	void RunFuncNoArgs(const char* moduleName, const char* funcName);

private:
	int Validate(void* func, const wchar_t *name);
	void ErrorMessage(const wchar_t* info);
	void InfoMessage(const wchar_t* info);
public:
	std::wstring Ansi2Unicode(const char* text);
	char* Unicode2Ansi(std::wstring text);

protected:
	HINSTANCE HDLL = NULL;

public:

std::wstring rootPath;
std::wstring dllPath;
std::wstring packagePath;
std::wstring homePath;
std::wstring scriptPath;

std::wstring path;

std::wstring dllName = L"python39.dll";
std::wstring zipName = L"python39.zip";
std::wstring scriptName = L"main";

wchar_t exePath[MAX_PATH];
wchar_t msg[1204];
};