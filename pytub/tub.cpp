#define _CRT_SECURE_NO_WARNINGS
#include "pytub.h"
#include <vector>

#define WINDOWS_EXE
//#undef WINDOWS_EXE

#ifdef WINDOWS_EXE
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"WinMainCRTStartup\"")
#endif // WINDOWS_EXE


FILE* FIN = NULL;
FILE* FOUT = NULL;

void AttachParentConsole()
{
	if (AttachConsole(ATTACH_PARENT_PROCESS))
	{
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
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

const char* InitScript =
"import sys\n"
"import os\n"
"try:\n"
"    fd = os.open('CONOUT$', os.O_RDWR | os.O_BINARY)\n"
"    fp = os.fdopen(fd, 'w')\n"
"    sys.stdout = fp\n"
"    sys.stderr = fp\n"
"except Exception as e:\n"
"    print(e)\n";


#ifdef WINDOWS_EXE
int WINAPI
WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int show)
{
	AttachParentConsole();
	ClearConsole();
#else
int main(int argc, char* argv[])
{
#endif // WINDOWS_EXE

	PyTub pytub = PyTub();
	//if (argc==2)
	//{
	//	pytub.scriptName = pytub.Ansi2Unicode(argv[2]);
	//}
#ifdef WINDOWS_EXE
	pytub.RunString(InitScript);;
#endif
	// auto run main.py
	std::vector<std::wstring>extension;
	extension.push_back(L".py");
	// Binaries are not currently supported 
	// After that, you can try to run directly with Py_Eval ...
	//extension.push_back(L".pyc")
	extension.push_back(L".ini");

	std::vector<std::wstring>rootDir;
	rootDir.push_back(pytub.homePath);
	rootDir.push_back(pytub.rootPath);
	rootDir.push_back(pytub.scriptPath);
	

	for (int i = 0; i < rootDir.size(); i++)
	{
		for (int j = 0; j < extension.size(); j++)
		{
			wchar_t pyPath[MAX_PATH] = { L"\0" };
			StrCpyW(pyPath, rootDir[i].c_str());

			std::wstring pyName = pytub.scriptName + extension[j];
			PathAppend(pyPath, pyName.c_str());
			pytub.RunFile(pytub.Unicode2Ansi(pyPath));
		}
	}
	
	pytub.PythonEnvFinish();
	printf("Exit Application...\n");


#ifdef WINDOWS_EXE
	DetachParentConsole();
#endif // WINDOWS_EXE
}