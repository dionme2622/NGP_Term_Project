#pragma once
#include "Map.h"
#include "Scene.h"



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

	virtual void SendData();
	virtual void ReceiveData();

	virtual bool IsServerConnected() { return true; };

private:
	HBITMAP				 backgroundImage;

	HBITMAP				 selecMapImage;
	bool				 showSelectMap = false;

	HBITMAP				 mapImage;
	HBITMAP*			 mapImages;

	HBITMAP				 playerImage[2];


	SSendLobbyPacket	 sendLobbyPacket;
	SRecvLobbyPacket     recvLobbyPacket;

	int pastData = -1;
	int pastCallData = -1;


<<<<<<< HEAD
	float xPosF = 0.0f;
	int xPos	= 0;

	bool pause;
=======

>>>>>>> parent of 44d4b8a (수정)
};




