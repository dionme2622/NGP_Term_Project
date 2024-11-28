#pragma once
#include "Scene.h"

class CPlayScene : public CScene
{
public:
	CPlayScene(HWND _hWnd, HINSTANCE _hInst, CGameFramework* GameFramework);
	~CPlayScene();

	virtual void Initialize();

	virtual void ProcessInput();
	int GetPressedKeysAsChar();
	int GetPressedKey();

	virtual void Update(float fTimeElapsed);
	virtual void Render();


	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	virtual void SendData(SOCKET _sock);
	virtual void ReceiveData(SOCKET _sock);
private:
	HBITMAP				backgroundImage;
	HBITMAP				TileBitmap1;

	//CPlayer*			player;			// 플레이어


	SOCKET sock;


	int pastData = -1;


};
