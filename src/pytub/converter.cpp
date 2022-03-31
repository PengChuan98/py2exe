#include "converter.h"


std::wstring Ansi2Unicode(const char* text)
{
	size_t require = strlen(text) * 2 + 10;
	std::wstring wide;
	wide.resize(require + 2);
	MultiByteToWideChar(CP_ACP, 0, text, -1, &wide[0], (int)require);
	size_t size = wcslen(wide.c_str());
	wide.resize(size);
	return wide;
}

char* Unicode2Ansi(std::wstring text)
{
	char* result = NULL;
	int size = 0;
	size = WideCharToMultiByte(CP_ACP, 0, text.c_str(), -1, NULL, 0, NULL, NULL);
	result = (char*)malloc((static_cast<unsigned long long>(size) + 1) * sizeof(char));

	WideCharToMultiByte(CP_ACP, 0, text.c_str(), -1, result, size, NULL, NULL);
	return result;

}

char* Unicode2Ansi(const wchar_t* text)
{
	char* result = NULL;
	int size = 0;
	size = WideCharToMultiByte(CP_ACP, 0, text, -1, NULL, 0, NULL, NULL);
	result = (char*)malloc((static_cast<unsigned long long>(size) + 1) * sizeof(char));

	WideCharToMultiByte(CP_ACP, 0, text, -1, result, size, NULL, NULL);
	return result;
}
