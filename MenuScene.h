#pragma once
#include "Scene.h"



class CMenuScene : public CScene
{
public:
	CMenuScene(CGameFramework* GameFramework);
	~CMenuScene();


	virtual void Initialize(HWND hwnd, HINSTANCE g_hInst);

	virtual void ProcessInput();

	virtual void Update();
	virtual void Render();

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);


private:
	HBITMAP			 backgroundImage;
	SButton			 Button_Help, Button_Play, Button_Quit;

	HBITMAP			 tutoralBtm;
	bool			 drawTutorial;

	SButton			 tutoExitButton;
};
