#pragma once
#pragma once

#include <istream>
#include <winsock2.h>

#include <windows.h>
#include <vector>
#include <string>

#include <ws2tcpip.h>


#include "random"
#pragma comment (lib, "msimg32.lib")
#pragma comment(lib, "Ws2_32.lib")  // Winsock ���̺귯�� �߰�

#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ����


#define FRAME_BUFFER_WIDTH		1220
#define FRAME_BUFFER_HEIGHT		950

// ���� ���
#define SERVERPORT 9000
#define BUFSIZE 50
#define DIR_LEFT		0x04
#define DIR_RIGHT		0x08
#define DIR_UP			0x10
#define DIR_DOWN		0x20

enum {
	LIVE = 0, DAMAGE, DEAD, ESCAPE
};

