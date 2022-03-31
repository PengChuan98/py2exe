#pragma once

#ifdef _DEBUG
#undef _DEBUG
#endif // _DEBUG

#include<Windows.h>
#include <clocale>
#include <string>

std::wstring Ansi2Unicode(const char* text);
char* Unicode2Ansi(std::wstring text);
char* Unicode2Ansi(const wchar_t* text);

