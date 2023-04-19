#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include <string.h>
#include <winuser.h>
#include <tlhelp32.h>
#include <iostream>
#ifdef _DEBUG
#define TRACE(x)  OutputDebugString(x)  // 输出到Debug窗口
#else
#define TRACE(x)  
#endif
