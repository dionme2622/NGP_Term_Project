#pragma once
#include "stdafx.h"
#include "Scene.h"
#include "GameFramework.h"



class CStartScene : public CScene
{
public:
	CStartScene();
	~CStartScene();



	virtual void Initialize(HWND hwnd, HINSTANCE g_hInst);

	virtual void ProcessInput();

	virtual void Update();
	virtual void Render();

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

private:

};
