#pragma once

#include <istream>
#include <windows.h>
#include "tchar.h"
#include "Resource.h"
#include "random"
#pragma comment (lib, "msimg32.lib")
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")



#define FRAME_BUFFER_WIDTH		1220
#define FRAME_BUFFER_HEIGHT		950

#define STARTSCENE		0
#define MENUSCENE		1
#define LOBBYSCENE		2
#define PLAYSCENE		3

#define BUTTON_HELP		200
#define BUTTON_PLAY		201
#define BUTTON_START	202

#define DIR_LEFT		0x04
#define DIR_RIGHT		0x08
#define DIR_UP			0x10
#define DIR_DOWN		0x20

enum {
	LIVE = 0, DAMAGE, DEAD, ESCAPE
};

