#pragma once

#include <istream>
#include <winsock2.h>

#include <windows.h>
#include <vector>
#include <string>

#include <ws2tcpip.h>


#include "tchar.h"
#include "Resource.h"

#pragma comment (lib, "msimg32.lib")
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#pragma comment(lib, "Ws2_32.lib")  // Winsock 라이브러리 추가


#define FRAME_BUFFER_WIDTH		1220
#define FRAME_BUFFER_HEIGHT		950

#define STARTSCENE 0
#define MENUSCENE 1
#define LOBBYSCENE 2
#define PLAYSCENE 3




#define BUTTON_HELP 200
#define BUTTON_PLAY 201
#define BUTTON_START 202



// 서버 통신
#define SERVERPORT 9000
#define BUFSIZE 50



