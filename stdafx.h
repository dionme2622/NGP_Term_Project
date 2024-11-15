#pragma once

#include <istream>
#include <windows.h>
#include "tchar.h"
#include <vector>
#include <string>

#include "Resource.h"

#pragma comment (lib, "msimg32.lib")
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")



#define FRAME_BUFFER_WIDTH		1220
#define FRAME_BUFFER_HEIGHT		950

#define STARTSCENE 0
#define MENUSCENE 1
#define LOBBYSCENE 2
#define PLAYSCENE 3




#define BUTTON_HELP 200
#define BUTTON_PLAY 201
#define BUTTON_START 202



// ���� ���
#define SERVERPORT 9000
#define BUFSIZE 50