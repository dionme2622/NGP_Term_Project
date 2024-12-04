#pragma once
#include "Scene.h"



class CMenuScene : public CScene
{
public:
	CMenuScene(HWND _hWnd, HINSTANCE _hInst, CGameFramework* GameFramework);
	~CMenuScene();


	virtual void Initialize();

	virtual void ProcessInput();

	virtual void Update(float fTimeElapsed);
	virtual void Render();

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	virtual void SendData() {};
	virtual void ReceiveData() {};

	virtual bool IsServerConnected() { return m_bServerConnected; };

	void Login();
private:
	HBITMAP			 backgroundImage;
	SButton			 Button_Help, Button_Play, Button_Quit;

	HBITMAP			 tutoralBtm;
	bool			 drawTutorial;

	SButton			 tutoExitButton;

};
