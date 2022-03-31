#include "pytub.h"

PyTub::PyTub()
{
}

PyTub::~PyTub()
{
}

bool PyTub::InitRuntime()
{
	Py_Initialize();
	if (!Py_IsInitialized())
	{
		Logging::Error("Python environment initialization completed.");
		return false;
	}
	Logging::Info("Python environment initialization completed.");
	return true;
}
