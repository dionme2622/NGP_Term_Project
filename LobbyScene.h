#pragma once
#include "Scene.h"


class CLobbyScene : public CScene
{
public:
	CLobbyScene(CGameFramework* GameFramework);
	~CLobbyScene();

	virtual void Initialize(HWND hwnd, HINSTANCE g_hInst);

	virtual void ProcessInput();

	virtual void Update(float fTimeElapsed);
	virtual void Render();

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	HBITMAP* SelectMap();
private:
	HBITMAP			 backgroundImage;

	HBITMAP			 selecMapImage;
	bool			 showSelectMap = false;

	HBITMAP			 mapImage;
	HBITMAP*		 mapImages;

};




