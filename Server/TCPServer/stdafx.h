#pragma once
#pragma once

#include <istream>
#include <winsock2.h>

#include <windows.h>
#include <vector>
#include <string>

#include <ws2tcpip.h>

#include "tchar.h"
#include "random"
#include "map"
#pragma comment (lib, "msimg32.lib")
#pragma comment(lib, "Ws2_32.lib")  // Winsock ���̺귯�� �߰�

#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ����


#define FRAME_BUFFER_WIDTH		1220
#define FRAME_BUFFER_HEIGHT		950

// ���� ���
#define SERVERPORT 9000
#define BUFSIZE 50
#define DIR_LEFT		37
#define DIR_RIGHT		39
#define DIR_UP			38
#define DIR_DOWN		40
#define DIR_STOP		48
#define SPACE			32
enum {
	LIVE = 0, DAMAGE, DEAD, ESCAPE
};

