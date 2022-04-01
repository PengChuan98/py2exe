#include "pytub.h"
#include <vector>

#ifdef WINDOWS_
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
int WINAPI
main(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int show)
{
	AttachParentConsole();
	ClearConsole();
	Logging::Console = false;
#else
int main(int argc, char* argv[])
{
	Logging::Console = true;
#endif // WINDOWS_

	/* TODO : parse parameter */
	Logging::Level = 2;
	PyTub pytub = PyTub();
	pytub.InitRuntime();
	pytub.Print();

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
	
	exit(0);
}
