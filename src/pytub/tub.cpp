#define _CRT_SECURE_NO_WARNINGS
#include "pytub.h"
#include <vector>
#include <iostream>

void GetCommand(PyTub* pytub);

#ifdef WINDOWS_
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
int WINAPI
main(HINSTANCE hInst, HINSTANCE hPrevInst, char* args[], int show)
//main(int argc, char* argv[])
{
	AttachParentConsole();
	ClearConsole();
	Logging::Console = false;
#else
int main(int argc, char* args[])
{
	Logging::Console = true;
#endif // WINDOWS_

	/* TODO : parse parameter */
	Logging::Level = 2;
	PyTub pytub = PyTub();
	pytub.InitRuntime();
	pytub.Print();

	//GetCommand(&pytub);

	if (pytub.GetFlag())
	{
		vector<wstring> dirs;
		dirs.push_back(pytub.CurrentPath);
		dirs.push_back(pytub.ScriptPath);
		dirs.push_back(pytub.PackagesPath);
		dirs.push_back(pytub.RootPath);

		wchar_t path[1024] = { L"\0" };
		wchar_t msg[1024] = { L"\0" };
		bool flag = false;

		for (size_t i = 0; i < dirs.size(); i++)
		{
			// TODO : append more type file,like .pyc .pyd
			wsprintf(path, L"%ws\\%ws.py", dirs[i].c_str(), pytub.EntryName.c_str());
			if (!PathFileExists(path))
			{
				continue;
			}
			flag = true;
			break;
		}
		// xxx.py not exists
		if (!flag)
		{
			wstring paths = L"";
			for (int i = 0; i < dirs.size(); i++)
			{
				paths += (L"\n ->" + dirs[i]);
			}
			wsprintf(msg, L"%ws.py is no longer in the following folder:%ws", pytub.EntryName.c_str(), paths.c_str());
			Logging::Info(msg);
		}
		else {
			pytub.RunFile(path);
		}

	}

#ifdef WINDOWS_
	Logging::Info("Detach Parent Console");
	DetachParentConsole();
#endif // WINDOWS_
	
	return 0;
}

void GetCommand(PyTub* pytub)
{
	LPWSTR* _argvw;
	int _argc;
	wstring _args;
	_args = GetCommandLineW();
	_argvw = CommandLineToArgvW(_args.c_str(), &_argc);
	if (_argvw == NULL) {
		Logging::Error("Error in CommandLineToArgvW()");
		return;
	}

	 //In the WinMain mode, the py_func of Py_Main has been wrong, the reason is unknown
	if (RedirectConsoleIO())
	{
		printf("debug 02\n");
		pytub->InteractiveMode();
	}
	else
	{
		Logging::Error("RedirectConsoleIO ERROR!");
	}
	// using PyRun_SimpleString
	//if (1)
	//if (_argc == 2 && StrCmp(_argvw[1], L"-c") == 0)
	{
		
	}
}
