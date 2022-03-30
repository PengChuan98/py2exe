#include "pytub.h"

void PyTub::_InitParameter(const wchar_t* path)
{
	// setting local language for Chinese-Unicode
	setlocale(LC_ALL, "chs");

	// auto setting runtime path
	rootPath = path;
	dllPath = rootPath + L"\\DLLS";
	packagePath = rootPath + L"\\site-packages";
	homePath = rootPath;
	scriptPath = rootPath + L"\\script";
}

PyTub::PyTub()
{
	// abspath of exe
	GetModuleFileName(NULL, exePath, MAX_PATH);
	PathRemoveFileSpec(exePath);
	_InitParameter(exePath);

	AutoSetPath();
	LoadPythonDll();
	PythonEnvInit();
}

PyTub::PyTub(const wchar_t* path)
{
	_InitParameter(path);
}

PyTub::~PyTub()
{
	// 这里应该是一直没有处理PyError的错误，导致直接Finlize会出错
	// 这里好像是python自己的bug。。。。
	// https://docs.python.org/3/c-api/init.html?highlight=py_finalize#c.Py_Finalize
	// 后面测试放在析构函数就会出现这个？可能是回收顺序？ MVSC在析构函数之前把我的python环境回收了？
	// 
	//if (flag)
	//{
	//	wsprintf(msg, L"Run Func Py_Finalize.");
	//	InfoMessage(msg);
		//if (PyErr_Occurred())
		//{
		//	PyErr_Print();
		//	PyErr_Clear();
		//}
		//Py_Finalize();
	//}
}


void PyTub::Py_DECREF(void* obj)
{

	Py_Dealloc(obj);
	//if (obj != *Py_None)
	//{
	//	
	//}
}

/// <summary>
/// load function from dll.
/// </summary>
/// <returns></returns>
bool PyTub::LoadPythonDll()
{
	SetCurrentDirectoryW(dllPath.c_str());
	HDLL = (HINSTANCE)LoadLibraryW(dllName.c_str());

	if (HDLL) {
		Py_Initialize = (_Py_Initialize)GetProcAddress(HDLL, "Py_Initialize");
		Py_IsInitialized = (_Py_IsInitialized)GetProcAddress(HDLL, "Py_IsInitialized");
		Py_Finalize = (_Py_Finalize)GetProcAddress(HDLL, "Py_Finalize");

		Py_SetPythonHome = (_Py_SetPythonHome)GetProcAddress(HDLL, "Py_SetPythonHome");
		Py_SetPath = (_Py_SetPath)GetProcAddress(HDLL, "Py_SetPath");

		Py_GetPath = (_Py_GetPath)GetProcAddress(HDLL, "Py_GetPath");
		Py_GetProgramFullPath = (_Py_GetProgramFullPath)GetProcAddress(HDLL, "Py_GetProgramFullPath");

		PyRun_SimpleString = (_PyRun_SimpleString)GetProcAddress(HDLL, "PyRun_SimpleString");
		
		//Py_XDECREF = (_Py_XDECREF)GetProcAddress(HDLL, "Py_XDECREF");
		//Py_DECREF = (_Py_DECREF)GetProcAddress(HDLL, "Py_DECREF");
		Py_Dealloc =(_Py_Dealloc)GetProcAddress(HDLL, "_Py_Dealloc");

		PyErr_Print = (_PyErr_Print)GetProcAddress(HDLL, "PyErr_Print");
		PyErr_Occurred = (_PyErr_Occurred)GetProcAddress(HDLL, "PyErr_Occurred");
		PyErr_Clear = (_PyErr_Clear)GetProcAddress(HDLL, "PyErr_Clear");

		PyObject_Print = (_PyObject_Print)GetProcAddress(HDLL, "PyObject_Print");

		PyUnicode_DecodeFSDefault = (_PyUnicode_DecodeFSDefault)GetProcAddress(HDLL, "PyUnicode_DecodeFSDefault");
		PyImport_Import = (_PyImport_Import)GetProcAddress(HDLL, "PyImport_Import");
		PyObject_GetAttrString = (_PyObject_GetAttrString)GetProcAddress(HDLL, "PyObject_GetAttrString");
		PyCallable_Check = (_PyCallable_Check)GetProcAddress(HDLL, "PyCallable_Check");
		PyObject_CallObject = (_PyObject_CallObject)GetProcAddress(HDLL, "PyObject_CallObject");

		Py_BuildValue = (_Py_BuildValue)GetProcAddress(HDLL, "Py_BuildValue");
		Py_FOpen = (_Py_fopen_obj)GetProcAddress(HDLL, "_Py_fopen_obj");
		PyRun_SimpleFile = (_PyRun_SimpleFile)GetProcAddress(HDLL, "PyRun_SimpleFile");

		//Py_NoneStruct = (_Py_NoneStruct)GetProcAddress(HDLL, "_Py_NoneStruct");
	}
	
	if (HDLL==NULL)
	{
		wsprintf(msg, L"The application can`t locate %ws from \"%ws \"", dllName.c_str(), dllPath.c_str());
		ErrorMessage(msg);
		return false;
	}
	else
	{
		int feedback = 0;
		feedback += Validate(Py_Initialize, L"Py_Initialize");
		feedback += Validate(Py_IsInitialized, L"Py_IsInitialized");
		feedback += Validate(Py_IsInitialized, L"Py_Finalize");

		feedback += Validate(Py_SetPythonHome, L"Py_SetPythonHome");
		feedback += Validate(Py_SetPath, L"Py_SetPath");

		feedback += Validate(Py_GetPath, L"Py_GetPath");
		feedback += Validate(Py_GetProgramFullPath, L"Py_GetProgramFullPath");

		feedback += Validate(PyRun_SimpleString, L"PyRun_SimpleString");
		
		//feedback += validate(Py_XDECREF, L"Py_XDECREF");
		//feedback += validate(Py_DECREF, L"Py_DECREF");
		feedback += Validate(Py_Dealloc, L"_Py_Dealloc");

		feedback += Validate(PyErr_Print, L"PyErr_Print");
		feedback += Validate(PyErr_Occurred, L"PyErr_Occurred");
		feedback += Validate(PyErr_Occurred, L"PyErr_Clear");

		feedback += Validate(PyObject_Print, L"PyObject_Print");

		feedback += Validate(PyUnicode_DecodeFSDefault, L"PyUnicode_DecodeFSDefault");
		feedback += Validate(PyImport_Import, L"PyImport_Import");
		feedback += Validate(PyObject_GetAttrString, L"PyObject_GetAttrString");
		feedback += Validate(PyCallable_Check, L"PyCallable_Check");
		feedback += Validate(PyObject_CallObject, L"PyObject_CallObject");

		feedback += Validate(Py_BuildValue, L"Py_BuildValue");
		feedback += Validate(Py_FOpen, L"_Py_fopen_obj");
		feedback += Validate(PyRun_SimpleFile, L"PyRun_SimpleFile");

		//feedback += Validate(Py_NoneStruct, L"_Py_NoneStruct");

		if (feedback!=0)
		{
			return false;
		}
		
		wsprintf(msg, L"Loaded All DLl.");
		InfoMessage(msg);
	
		return true;
	}

	//return false;
}

void PyTub::AutoSetPath()
{
	// add zip file modules
	path.append(rootPath + L"\\" + zipName+L";");
	path.append(dllPath + L"\\" + zipName+L";");
	path.append(packagePath + L"\\" + zipName+L";");
	
	path.append(rootPath + L";");
	path.append(dllPath + L";");
	path.append(packagePath + L";");
	path.append(scriptPath + L";");

	path.append(homePath + L";");
	path.append(homePath + L"\\" + zipName + L";");

	wsprintf(msg, L"Runing Func AutoSetPath.");
	InfoMessage(msg);
}

bool PyTub::PythonEnvInit()
{
	Py_SetPythonHome(homePath.c_str());
	
	if (!path.empty())
	{
		Py_SetPath(path.c_str());
	}
	
	Py_Initialize();
	if (!Py_IsInitialized())
	{
		wsprintf(msg, L"PythonEnv is failed.");
		ErrorMessage(msg);
		return false;
	}

	wsprintf(msg, L"Python Env is initialized.");
	InfoMessage(msg);

	return true;
}

void PyTub::PythonEnvFinish()
{
	wsprintf(msg, L"Run Func Py_Finalize.");
	InfoMessage(msg);
	Py_Finalize();
}

void PyTub::RunString(const char* command)
{
	PyRun_SimpleString(command);
}

void PyTub::RunFile(const char* path)
{
	wchar_t wpath[MAX_PATH] = { L"\0" };
	StrCpyW(wpath, Ansi2Unicode(path).c_str());

	if (!PathFileExistsA(path))
	{
		wsprintf(msg, L"Can`t find file: %ws.", wpath);
		ErrorMessage(msg);
		return;
	}

	void* obj = Py_BuildValue("s", path);
	void* fp = Py_FOpen(obj, "r");

	if (fp == NULL)
	{
		wsprintf(msg, L"Open [%ws] error.", wpath);
		ErrorMessage(msg);
		return;
	}

	wsprintf(msg, L"Run File %ws", wpath);
	InfoMessage(msg);

	PyRun_SimpleFile(fp, PathFindFileNameA(path));
	Py_DECREF(obj);
}

void PyTub::RunFuncNoArgs(const char* moduleName, const char* funcName)
{
	void* pName, * pModule, * pFunc;
	void* pValue;

	pName = PyUnicode_DecodeFSDefault(moduleName);

	// load module and decref
	pModule = PyImport_Import(pName);
	Py_DECREF(pName);

	if (pModule != NULL) {
		// load func object

		pFunc = PyObject_GetAttrString(pModule, funcName);

		if (pFunc && PyCallable_Check(pFunc)) {

			pValue = PyObject_CallObject(pFunc, NULL);
			// this func not support return and args.
			if (pValue != NULL) {
				// load func successed.
				if (pValue != NULL)
				{
					PyObject_Print(pValue, stdout, 0);
				}
				Py_DECREF(pValue);
			}
			else {
				Py_DECREF(pFunc);
				Py_DECREF(pModule);
				PyErr_Print();
				wsprintf(msg, L"Call failed\n");
				ErrorMessage(msg);
				return;
			}
		}
		else {
			if (PyErr_Occurred())
				PyErr_Print();
			wsprintf(msg, L"Cannot find function \"%ws\"\n", Ansi2Unicode(funcName).c_str());
			ErrorMessage(msg);
		}

		Py_DECREF(pFunc);
		Py_DECREF(pModule);
		
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="func"></param>
/// <param name="name"></param>
/// <returns></returns>
int PyTub::Validate(void* func, const wchar_t* name)
{
	if (func == NULL)
	{
		wsprintf(msg, L"Can`t find %ws in %ws.", name, dllName.c_str());
		ErrorMessage(msg);
		return 1;
	}
	return 0;
}

#define CONSOLE_MSG
void PyTub::ErrorMessage(const wchar_t* info)
{
#ifdef CONSOLE_MSG
	printf("[ - ERROR - ]\t%ws\n", info);
#else
	MessageBoxW(NULL, info, NULL, MB_OK | MB_ICONERROR);
	//flag = false;
#endif CONSOLE_MSG
}

void PyTub::InfoMessage(const wchar_t* info)
{
#ifdef CONSOLE_MSG
	printf("[ - INFO - ]\t%ws\n", info);
#endif CONSOLE_MSG
}

std::wstring PyTub::Ansi2Unicode(const char* text)
{
	int require = (int)strlen(text) * 2 + 10;
	std::wstring wide;
	wide.resize(require + 2);
	MultiByteToWideChar(CP_ACP, 0, text, -1, &wide[0], require);
	int size = wcslen(wide.c_str());
	wide.resize(size);
	return wide;
}

char* PyTub::Unicode2Ansi(std::wstring text)
{
	char* result = NULL;
	int size = 0;
	size = WideCharToMultiByte(CP_ACP, 0, text.c_str(), -1, NULL, 0, NULL, NULL);
	result = (char*)malloc((size + 1) * sizeof(char));

	WideCharToMultiByte(CP_ACP, 0, text.c_str(), -1, result, size, NULL, NULL);
	return result;

}
