// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once

#include "targetver.h"
//#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
// Misc
#include <string>
// Buffer
#include <Uxtheme.h>
#pragma comment (lib, "uxtheme.lib")
// Gdiplus
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")