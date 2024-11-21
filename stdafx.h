#pragma once

#include <istream>
#include <winsock2.h>

#include <windows.h>
#include <vector>
#include <string>

#include <ws2tcpip.h>


#include "tchar.h"
#include "Resource.h"
#include "random"
#pragma comment (lib, "msimg32.lib")
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#pragma comment(lib, "Ws2_32.lib")  // Winsock 라이브러리 추가

#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기


#define FRAME_BUFFER_WIDTH		1220
#define FRAME_BUFFER_HEIGHT		950

#define STARTSCENE		0
#define MENUSCENE		1
#define LOBBYSCENE		2
#define PLAYSCENE		3

#define BUTTON_HELP		200
#define BUTTON_PLAY		201
#define BUTTON_START	202

#define BUTTON_HELP 200
#define BUTTON_PLAY 201
#define BUTTON_START 202



// 서버 통신
#define SERVERPORT 9000
#define BUFSIZE 50
#define DIR_LEFT		37
#define DIR_RIGHT		39
#define DIR_UP			38
#define DIR_DOWN		40

enum {
	LIVE = 0, DAMAGE, DEAD, ESCAPE
};

