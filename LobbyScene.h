#pragma once
#include "Map.h"
#include "Scene.h"

typedef struct PacketHeader {
	int packetType; // 1=PlayerInput, 2=ReadyPacket
} PacketHeader;


struct SSendLobbyPacket {
	PacketHeader header;
	int selectedMap = 0;
	bool nextSceneCall = 0;
};

struct SRecvLobbyPacket {
	CMap recvMapData;
	bool recvNextSceneCall = false;
};

class CLobbyScene : public CScene
{
public:
	CLobbyScene(HWND _hWnd, HINSTANCE _hInst, CGameFramework* GameFramework);
	~CLobbyScene();

	virtual void Initialize();

	virtual void ProcessInput();

	virtual void Update(float fTimeElapsed);
	virtual void Render();

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void SelectMap();

	virtual void SendData(SOCKET _sock);
	virtual void ReceiveData(SOCKET _sock);


private:
	HBITMAP				 backgroundImage;

	HBITMAP				 selecMapImage;
	bool				 showSelectMap = false;

	HBITMAP				 mapImage;
	HBITMAP*			 mapImages;

	SSendLobbyPacket	 sendLobbyPacket;
	SRecvLobbyPacket     recvLobbyPacket;
};




