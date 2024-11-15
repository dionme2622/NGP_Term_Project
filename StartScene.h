#pragma once
#include "stdafx.h"
#include "Scene.h"
#include "GameFramework.h"



class CStartScene : public CScene
{
public:
	CStartScene(HWND _hWnd, HINSTANCE _hInst, CGameFramework* GameFramework);
	~CStartScene();



	virtual void Initialize();

	virtual void ProcessInput();

	virtual void Update(float fTimeElapsed);
	virtual void Render();

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

private:

};
