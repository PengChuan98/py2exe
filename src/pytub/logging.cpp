#include "logging.h"

int Logging::Level = 1;
bool Logging::Console = true;

void Logging::Error(const wchar_t* info)
{
	if (Level<1)
	{
		return;
	}
	fprintf(stderr, " [ E ] \t%ws\n", info);

	if (!Console)
	{
		MessageBoxW(NULL, info, L"Error", MB_OK | MB_ICONERROR);
	}
}

void Logging::Error(const char* info)
{
	Error(Ansi2Unicode(info).c_str());
}

void Logging::Info(const wchar_t* info)
{
	Info(info, false);
}

void Logging::Info(const wchar_t* info, bool show)
{
	if (Level < 2)
	{
		return;
	}
	fprintf(stdout, " [ I ] \t%ws\n", info);

	if (show)
	{
		MessageBoxW(NULL, info, L"Info", MB_OK);
	}
}

void Logging::Info(const char* info)
{
	Info(Ansi2Unicode(info).c_str());
}
