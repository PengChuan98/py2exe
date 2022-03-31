#ifdef TEST_MODE

#include "pytub.h"
#include <vector>

void line() {
	printf("------------------------------------------------------------\n");
}

void print(PyTub pt) {
	line();
	wprintf(L"rootPath:\t%ws\n", pt.rootPath.c_str());
	wprintf(L"dllPath:\t%ws\n", pt.dllPath.c_str());
	wprintf(L"homePath:\t%ws\n", pt.homePath.c_str());
	wprintf(L"packagePath:\t%ws\n", pt.packagePath.c_str());
	wprintf(L"scriptPath:\t%ws\n", pt.scriptPath.c_str());
	wprintf(L"exePath:\t%ws\n", pt.exePath);
	wprintf(L"path:\t%ws\n", pt.path.c_str());
	line();
}

void pp(const char* info) {
	printf("-----------------------");
	printf("%s", info);
	printf("-----------------------\n");
}

void main()
#endif // TEST_MODE