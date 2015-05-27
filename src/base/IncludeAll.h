#ifndef _INCLUDE_ALL_H_
#define _INCLUDE_ALL_H_

#define DIRECTINPUT_VERSION 0x0800

#define _CRT_SECURE_NO_DEPRECATE

#define UNICODE
#define _UNICODE

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <share.h>

#include "DefineDirectX.h"

#include <dinput.h>

#include"define.h"


//#define _using_boost
#ifdef _using_boost
#include<boost/thread.hpp>
#include<boost/bind.hpp>
using namespace boost;
#define ScopedLock(x) boost::mutex::scoped_lock(x)
#else
#include<thread>
#include<mutex>
using namespace std;
#define ScopedLock(x) lock_guard<mutex> lock(x)
#endif


#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#include"SmartPointer.h"
#include"MyFunction.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
extern TCHAR* COURSE_NAME[];

#include"../ErrorManager/ErrorManager.h"

#endif