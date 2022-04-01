#define TESTFILE
#undef TESTFILE


#ifdef TESTFILE

#include "pytub.h"
#include <vector>


int main(int argc, char* argv[])
{
	Logging::Level = 2;
	printf("--------------------------------------------------\n");
	//SetCurrentDirectory(L"C:\\Users\\haeer\\Desktop\\python3\\embed");
	SetDllDirectory(L"C:\\Users\\haeer\\Desktop\\python3\\embed");

	PyTub pytub = PyTub();
	pytub.HomePath = L"C:\\Users\\haeer\\Desktop\\python3\\pure";
	pytub.InitRuntime();

	printf("--------------------------------------------------\n");


	pytub.Print();

	pytub.RunFile("C:\\Users\\haeer\\Desktop\\py2exe\\script\\main.py");

	//pytub.InteractiveMode(argc, argv);

	printf("--------------------------------------------------\n");

}
#endif // TESTFILE

