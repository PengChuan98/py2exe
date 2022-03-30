#pragma once
#include <stdio.h>

// initialize of python settings
typedef void(*_Py_Initialize)(void);
typedef int (*_Py_IsInitialized)();
typedef void (*_Py_Finalize)();

typedef void (*_Py_SetPythonHome)(const wchar_t* home);
typedef void (*_Py_SetPath)(const wchar_t*);

typedef wchar_t* (*_Py_GetPath)();
typedef wchar_t* (*_Py_GetProgramFullPath)();

// simple run
typedef int (*_PyRun_SimpleString)(const char* command);

// reference settings
// The following two functions are not externally packaged
typedef void (*_Py_XDECREF)(void* o);
typedef void (*_Py_DECREF)(void* o);
// The following function is only for use within the interpreter core
typedef void(*_Py_Dealloc)(void* obj);

// error
typedef void (*_PyErr_Print)();
typedef void* (*_PyErr_Occurred)();
typedef void (*_PyErr_Clear)();
// tools func
typedef int (*_PyObject_Print)(void* o, FILE* fp, int flags);

// run function of python module
typedef void* (*_PyUnicode_DecodeFSDefault)(const char* _char);
typedef void* (*_PyImport_Import)(void* name);
typedef void* (*_PyObject_GetAttrString)(void* o, const char* attr_name);
typedef int (*_PyCallable_Check)(void* o);
typedef void* (*_PyObject_CallObject)(void* callable, void* args);

// run python file
typedef void* (*_Py_BuildValue)(const char* format, ...);
typedef void* (*_Py_fopen_obj)(void* path, const char* mode);
typedef int (*_PyRun_SimpleFile)(void* fp, const char* filename);

// other
//typedef struct _Py_NoneStruct;
