#define _CRT_SECURE_NO_WARNINGS
#include "pytub.h"
#include <vector>

/// <summary>
/// Initialize class member variables .
/// </summary>
void PyTub::_InitParameter(wstring root)
{
	// setting local language for Chinese-Simplifid
	setlocale(LC_ALL, "chs");

	// auto setting runtime path
	RootPath = root;
	DLLPath = RootPath + L"\\DLLs";
	PackagesPath = RootPath + L"\\site-packages";
	ScriptPath = RootPath + L"\\script";

	GetCurrentDirectory(MAX_LENGTH, path);
	CurrentPath = path;

	wstring temp;
	vector<wstring> dirs;
	dirs.push_back(RootPath);
	dirs.push_back(DLLPath);

	flag = false;
	for (size_t i = 0; i < dirs.size(); i++)
	{
		wsprintf(path, L"%ws\\%ws", dirs[i].c_str(), dll);
		if (!PathFileExists(path))
		{
			continue;
		}
		temp = dirs[i];  // dll dir
		flag = true;
		break;
	}

	// dll is not exists.
	if (!flag)
	{
		wstring paths = L"";
		for (int i = 0; i < dirs.size(); i++)
		{
			paths += (L"\n -> " + dirs[i]);
		}
		wsprintf(msg, L"%ws is no longer in the following folder:%ws", dll, paths.c_str());
		Logging::Error(msg);
		return;
	}

	// reset Path
	DLLPath = temp;

	// set dll dir
	SetDllDirectory(DLLPath.c_str());

	PYPATH = L"";
	PYPATH += (DLLPath + L"\\" + zip + L";");
	PYPATH += (DLLPath + L";");
	PYPATH += (PackagesPath + L";");
	PYPATH += (ScriptPath + L";");
	PYPATH += (RootPath + L";");

	wsprintf(msg, L"Load %ws from %ws", dll, DLLPath.c_str());
	Logging::Info(msg);

	wsprintf(msg, L"PythonPath and DLL Directory are already set up.");
	Logging::Info(msg);
}


PyTub::PyTub()
{
	// abspath of exe
	GetModuleFileName(NULL, path, MAX_LENGTH);
	ProgramName = path;
	PathRemoveFileSpec(path);

	CurrentPath = path;
	_InitParameter(CurrentPath);
}

PyTub::PyTub(const wchar_t* p)
{
	CurrentPath = p;
	_InitParameter(CurrentPath);
}

PyTub::~PyTub()
{
	if (Py_IsInitialized())
	{
		Py_Finalize();
		Logging::Info("Undo all initializations completed.");
	}
}


bool PyTub::InitRuntime()
{
	if (!flag)
	{
		return false;
	}
	Py_SetProgramName(ProgramName.c_str());
	Py_SetPythonHome(RootPath.c_str());
	Py_SetPath(PYPATH.c_str());
	//Py_SetPath(DLLPath.c_str());

	Py_Initialize();
	if (!Py_IsInitialized())
	{
		Logging::Error("Python environment initialization completed.");
		return false;
	}
	Logging::Info("Python environment initialization completed.");

	// init some setting config
	const char* script_string =
		"import site\n"
		"import os\n"
		"import sys\n"
#ifdef WINDOWS_
		"try:\n"
		"    fd = os.open('CONOUT$', os.O_RDWR | os.O_BINARY)\n"
		"    fp = os.fdopen(fd, 'w')\n"
		"    sys.stdout = fp\n"
		"    sys.stderr = fp\n"
		"except Exception as e:\n"
		"    print('[ E ] [Python] \t', e)\n"
#endif
		"";    // !!! only the last line is empty

		if (!RunString(script_string))
		{
			Logging::Error("Python script_string initialization failed.");
		}
	
		Logging::Info("Python script_string initialization completed.");
		return true;
}

void PyTub::AppendPath(wchar_t* p)
{
	PYPATH.append(p);
	PYPATH.append(L";");
}

bool PyTub::AfterAppendPath(wchar_t* p)
{
	wsprintf(command, L"sys.path.append(r'%ws')", p);
	return RunString(command);
}

bool PyTub::RunString(const char* line)
{
	if (!flag)
	{
		return false;
	}
	PyRun_SimpleString(line);
	return true;
}

bool PyTub::RunString(const wchar_t* line)
{
	return RunString(Py_EncodeLocale(line, NULL));
}

bool PyTub::RunFile(const char* file)
{
	return RunFile(Py_DecodeLocale(file, NULL));
}

/// <summary>
/// ´ý²âÊÔ
/// </summary>
/// <param name="file"></param>
/// <returns></returns>
bool PyTub::RunFile(const wchar_t* file)
{
	if (!flag)
	{
		return false;
	}

	if (!PathFileExists(file))
	{
		wsprintf(msg, L"Can`t find file: %ws.", file);
		Logging::Error(msg);
		return false;
	}

	PyObject* _path = Py_BuildValue("s", Py_EncodeLocale(file, NULL));
	FILE* fp = _Py_fopen_obj(_path, "rb");
	
	if (fp==NULL)
	{
		Py_XDECREF(_path);

		wsprintf(msg, L"Open file : [%ws] error.", file);
		Logging::Error(msg);
		return false;
	}

	wsprintf(msg, L"Loading File %ws", file);
	Logging::Info(msg);

	int flag = PyRun_SimpleFile(fp, PathFindFileNameA(Py_EncodeLocale(file, NULL)));

	wsprintf(msg, L"FileRun return %d", flag);
	Logging::Info(msg);

	Py_DECREF(_path);
	return true;
}

bool PyTub::RunByteFile(const wchar_t* file)
{
	return false;
}

bool PyTub::InteractiveMode(int argc, wchar_t** argv)
{
	if (!flag)
	{
		return false;
	}

	try
	{
		Py_Main(argc, argv);
		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}

/// <summary>
/// TODO : using muti-process or muti-thread
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
bool PyTub::InteractiveMode(int argc, char** argv)
{
	if (!flag)
	{
		return false;
	}
	vector<wchar_t*> v_argv;
	vector<wstring> v_argc;

	v_argc.resize(0);
	for (int i = 0; i < argc; i++)
	{
		v_argc.push_back(Py_DecodeLocale(argv[i], NULL));
	}
	v_argc.insert(v_argc.begin() + 1, L"-i");
	v_argc.insert(v_argc.begin() + 2, L"-I");
	v_argc.insert(v_argc.begin() + 3, L"-s");
	v_argc.insert(v_argc.begin() + 4, L"-S");
	v_argc.insert(v_argc.begin() + 5, L"-c");

	v_argv.resize(0);
	for (int i = 0; i < v_argc.size(); i++)
	{
		v_argv.push_back((wchar_t*)v_argc[i].c_str());
	}

	try
	{
		// TODO : when the following exits,the main exits also??
		Py_Main(argc, &v_argv[0]);
	}
	catch (const std::exception&)
	{
		return false;
	}

	return true;
}

bool PyTub::InteractiveMode()
{
	// load args
	LPWSTR* argvw;
	int argc;
	wstring args;

	args = GetCommandLineW();
	argvw = CommandLineToArgvW(args.c_str(), &argc);
	if (argvw == NULL) {
		Logging::Error("Error in CommandLineToArgvW()");
		return false;
	}
	
	for (int i = 0; i < argc; i++)
	{
		printf("-->%ws\n", argvw[i]);
	}

	return InteractiveMode(argc, argvw);
}


bool PyTub::SimpleConsole()
{
	if (!flag)
	{
		return false;
	}

	

	
	return true;
}


void PyTub::Print()
{
	printf("------------------------------------------------------------\n");
	printf(" RootPath\t->\t%ws\n", RootPath.c_str());
	printf(" PackagesPath\t->\t%ws\n", PackagesPath.c_str());
	printf(" DLLPath\t->\t%ws\n", DLLPath.c_str());
	printf(" ScriptPath\t->\t%ws\n", ScriptPath.c_str());
	printf(" ProgramName\t->\t%ws\n", ProgramName.c_str());
	printf(" CurrentPath\t->\t%ws\n", CurrentPath.c_str());
	printf(" EntryName\t->\t%ws\n", EntryName.c_str());
	printf("------------------------------------------------------------\n");
	
	if (flag)
	{
		printf(" PATHONPATH :\n");
		const char* cmd = "[print(' -> {}'.format(i)) for i in sys.path if len(i.strip())!=0]\n";
		RunString(cmd);
		printf("------------------------------------------------------------\n");
	}


}

bool PyTub::GetFlag()
{
	return flag;
}

void AttachParentConsole()
{
	//AllocConsole();
	//freopen("CONIN$", "r", stdin);
	//freopen("CONOUT$", "w", stdout);
	//freopen("CONOUT$", "w", stderr);
	FreeConsole();
	if (AttachConsole(ATTACH_PARENT_PROCESS))
	{
		freopen("CONIN$", "r+t", stdin);
		freopen("CONOUT$", "w+t", stdout);
		freopen("CONOUT$", "w+t", stderr);
		//int fd = _fileno(stdout);
		//if (fd >= 0) {
		//	std::string fn = std::to_string(fd);
		//	SetEnvironmentVariableA("PYSTAND_STDOUT", fn.c_str());
		//}
		//fd = _fileno(stdin);
		//if (fd >= 0) {
		//	std::string fn = std::to_string(fd);
		//	//SetEnvironmentVariableA("PYSTAND_STDIN", fn.c_str());
		//}
	}
	//FILE* inConsole, outConsole;
	//void* temp;
	//FIN = freopen("CONIN$", "r+t", stdin);
	//FOUT = freopen("CONOUT$", "w+t", stdout);
	//printf("");

}

void DetachParentConsole() {
	//if (FIN!=NULL)free(FIN);
	//if (FOUT != NULL)free(FOUT);
	FreeConsole();

}

void ClearConsole()
{
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD coordScreen = { 0, 0 };    // home for the cursor
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;

	// Get the number of character cells in the current buffer.
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		return;
	}

	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	// Fill the entire screen with blanks.
	if (!FillConsoleOutputCharacter(hConsole,        // Handle to console screen buffer
		(TCHAR)' ',      // Character to write to the buffer
		dwConSize,       // Number of cells to write
		coordScreen,     // Coordinates of first cell
		&cCharsWritten)) // Receive number of characters written
	{
		return;
	}

	// Get the current text attribute.
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		return;
	}

	// Set the buffer's attributes accordingly.
	if (!FillConsoleOutputAttribute(hConsole,         // Handle to console screen buffer
		csbi.wAttributes, // Character attributes to use
		dwConSize,        // Number of cells to set attribute
		coordScreen,      // Coordinates of first cell
		&cCharsWritten))  // Receive number of characters written
	{
		return;
	}

	// Put the cursor at its home coordinates.
	SetConsoleCursorPosition(hConsole, coordScreen);
}

bool RedirectConsoleIO()
{
	bool result = true;
	FILE* fp;

	// Redirect STDIN if the console has an input handle
	if (GetStdHandle(STD_INPUT_HANDLE) != INVALID_HANDLE_VALUE)
		if (freopen_s(&fp, "CONIN$", "r", stdin) != 0)
			result = false;
		else
			setvbuf(stdin, NULL, _IONBF, 0);

	// Redirect STDOUT if the console has an output handle
	if (GetStdHandle(STD_OUTPUT_HANDLE) != INVALID_HANDLE_VALUE)
		if (freopen_s(&fp, "CONOUT$", "w", stdout) != 0)
			result = false;
		else
			setvbuf(stdout, NULL, _IONBF, 0);

	// Redirect STDERR if the console has an error handle
	if (GetStdHandle(STD_ERROR_HANDLE) != INVALID_HANDLE_VALUE)
		if (freopen_s(&fp, "CONOUT$", "w", stderr) != 0)
			result = false;
		else
			setvbuf(stderr, NULL, _IONBF, 0);

	// Make C++ standard streams point to console as well.
	ios::sync_with_stdio(true);

	// Clear the error state for each of the C++ standard streams.
	std::wcout.clear();
	std::cout.clear();
	std::wcerr.clear();
	std::cerr.clear();
	std::wcin.clear();
	std::cin.clear();

	return result;
}