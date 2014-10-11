#pragma once
#include <Windows.h>

#include "WaveOut.h"
#pragma comment(lib,"Winmm")

#include <assert.h>
#define INP_BUFFER_SIZE 640

// #include "../myDebug/mydebug.h"
// #pragma comment(lib, "..\\lib\\mydebug.lib")
//#define AUDIODEBUG(str) MYDEBUG(str)
#define AUDIODEBUG(str)