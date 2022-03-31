#include "pytub.h"

//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"WinMainCRTStartup\"")


int main()
{
	Logging::Level = 2;
	printf("--------------------------------------------------\n");
	//SetDllDirectory(L"C:\\Users\\haeer\\Desktop\\python3\\pure");
	SetDllDirectory(L"C:\\Users\\haeer\\Desktop\\python3\\embed");

	PyTub pytub = PyTub();
	pytub.InitRuntime();

	printf("--------------------------------------------------\n");
}