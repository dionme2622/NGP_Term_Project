#pragma once
#include "Scene.h"

class CPlayScene : public CScene
{
public:
	CPlayScene(CGameFramework* GameFramework);
	~CPlayScene();

	virtual void Initialize(HWND hwnd, HINSTANCE g_hInst);

	virtual void ProcessInput();


	virtual void Update();
	virtual void Render();


	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);


private:
	HBITMAP			 backgroundImage;


	CPlayer* player;			// 플레이어

};
