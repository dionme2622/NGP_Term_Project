#pragma once
#include "Scene.h"


class CLobbyScene : public CScene
{
public:
	CLobbyScene(CGameFramework* GameFramework);
	~CLobbyScene();

	virtual void Initialize(HWND hwnd, HINSTANCE g_hInst);

	virtual void ProcessInput();

	virtual void Update();
	virtual void Render();

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void SelectMap();
private:
	HBITMAP			 backgroundImage;

	HBITMAP			 selecMapImage;
	bool			 showSelectMap = false;

	HBITMAP			 mapImage;
	HBITMAP*		 mapImages;

};




