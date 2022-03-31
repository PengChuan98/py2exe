#pragma once

#include "converter.h"


class Logging {
public:
	/// <summary>
	/// 0 - none
    /// 1 - only error 
    /// 2- total msg
	/// </summary>
	static int Level;

    /// <summary>
    /// console or windows
    /// </summary>
    static bool Console;

public:
    static void Error(const wchar_t* info);
    static void Error(const char* info);

    static void Info(const wchar_t* info);
	static void Info(const wchar_t* info, bool show);
    static void Info(const char* info);
};


