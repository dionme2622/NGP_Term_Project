#include "MenuScene.h"
#include "GameFramework.h"

CMenuScene::CMenuScene(HWND _hWnd, HINSTANCE _hInst, CGameFramework* GameFramework) : CScene(_hWnd, _hInst, GameFramework)
{
}

CMenuScene::~CMenuScene()
{
}

void CMenuScene::Initialize()
{
    

    drawTutorial = false;

    backgroundImage = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MENU));


    // 버튼 설정
    Button_Help.pos = { rc.right / 4 - 100, 700, 200, 100 };
    Button_Help.size = { 0, 0, 200, 100 };
    Button_Help.ButtonImage = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_HELPBUTTON));


    Button_Play = { rc.right / 2 - 100, 700, 200, 100 };
    Button_Play.size = { 0, 0, 200, 100 };
    Button_Play.ButtonImage = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PLAYBUTTON));


    Button_Quit = { rc.right / 2 + 200, 700, 200, 100 };
    Button_Quit.size = { 0, 0, 200, 100 };
    Button_Quit.ButtonImage = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_QUITBUTTON));


    tutoralBtm = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TUTO));

    tutoExitButton.pos = { 535, 560, 135, 40 };
    tutoExitButton.size = { 0, 0, 135, 40 };
    tutoExitButton.ButtonImage = NULL;

}
    



void CMenuScene::ProcessInput()
{
	static UCHAR pKeysBuffer[256];
	bool bProcessedByScene = false;
	GetKeyboardState(pKeysBuffer);
    if (pKeysBuffer[VK_UP] & 0xF0) exit(0);
}


void CMenuScene::Update(float fTimeElapsed)
{
    // TODO : Menu Scene Update

}



void CMenuScene::Render()
{
    HDC hdc = GetDC(hWnd);

    HDC MemDC = CreateCompatibleDC(hdc);
    HDC MemDCImage = CreateCompatibleDC(hdc);

    hBit = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
    OldBit[0] = (HBITMAP)SelectObject(MemDC, hBit);
    OldBit[1] = (HBITMAP)SelectObject(MemDCImage, backgroundImage); //--- 배경 이미지	
    StretchBlt(MemDC, 0, 0, rc.right, rc.bottom, MemDCImage, 0, 0, 1220, 950, SRCCOPY);

    // 버튼 세개
    OldBit[1] = (HBITMAP)SelectObject(MemDCImage, Button_Help.ButtonImage);
    DrawImage(MemDC, Button_Help.pos, MemDCImage, Button_Help.size, RGB(255, 0, 255));


    OldBit[1] = (HBITMAP)SelectObject(MemDCImage, Button_Play.ButtonImage);
    DrawImage(MemDC, Button_Play.pos, MemDCImage, Button_Play.size, RGB(255, 0, 255));


    OldBit[1] = (HBITMAP)SelectObject(MemDCImage, Button_Quit.ButtonImage);
    DrawImage(MemDC, Button_Quit.pos, MemDCImage, Button_Quit.size, RGB(255, 0, 255));


    if (drawTutorial) {
        OldBit[2] = (HBITMAP)SelectObject(MemDCImage, tutoralBtm);
        RECT tutoPos{ rc.right / 2 - 210 , rc.bottom / 2 - 176 , 420, 352 };
        RECT tutoSize{ 0, 0, 320, 252 };
        DrawImage(MemDC, tutoPos, MemDCImage, tutoSize, RGB(255, 0, 255));
    }
    


    BitBlt(hdc, 0, 0, rc.right, rc.bottom, MemDC, 0, 0, SRCCOPY);


    // 자원 해제
    SelectObject(MemDC, OldBit[0]);
    DeleteObject(hBit);
    DeleteDC(MemDC);
    DeleteDC(MemDCImage);
    ReleaseDC(hWnd, hdc);
}


bool IsCursorInButton(SButton button, POINT cursorPos)
{
    if (cursorPos.x > button.pos.left && cursorPos.x < button.pos.left + button.size.right&&
        cursorPos.y > button.pos.top && cursorPos.y < button.pos.top + button.size.bottom)
        return true;
    return false;
}


void CMenuScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
    switch (nMessageID)
    {
    case WM_LBUTTONDOWN:
        break;
    case WM_RBUTTONDOWN:
        break;
    case WM_LBUTTONUP:
        SetCapture(hWnd);
        GetCursorPos(&cursorPos);
        ScreenToClient(hWnd, &cursorPos);

        if (IsCursorInButton(Button_Help, cursorPos)) {
            drawTutorial = true;
        }
        else if (IsCursorInButton(Button_Play, cursorPos)) {
            if (Login()) {  // Login 성공 시에만 다음 씬으로 이동
                GetFramework()->SetCurScene(LOBBYSCENE);
            }
        }
        else if (IsCursorInButton(Button_Quit, cursorPos)) {
            exit(0);
        }

        if (drawTutorial && IsCursorInButton(tutoExitButton, cursorPos)) {
            drawTutorial = false;
        }
        break;
    case WM_RBUTTONUP:
        ::ReleaseCapture();
        break;
    case WM_MOUSEMOVE:
        break;
    default:
        break;
    }
}

INT_PTR CALLBACK IpInputDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

bool CMenuScene::Login()
{
    // 서버 통신 관련 변수 초기화
    // 윈속 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup 실패\n");
        return false;
    }

    // 소켓 생성
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("소켓 생성 실패\n");
        WSACleanup();
        return false;
    }

    char ipAddress[16] = "127.0.0.1";  // 기본값으로 로컬 IP 설정
    int response = DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_IP_DIALOG), hWnd, (DLGPROC)IpInputDialogProc, reinterpret_cast<LPARAM>(ipAddress));

    printf("사용자가 입력한 IP 주소: %s\n", ipAddress);  // IP 주소 확인용 출력

    // 사용자가 대화상자에서 취소를 누르면 함수 종료
    if (response == IDCANCEL) {
        closesocket(sock);
        WSACleanup();
        return false;
    }

    // IP 주소 유효성 검사 및 소켓 주소 구조체 초기화
    memset(&remoteAddr, 0, sizeof(remoteAddr));
    remoteAddr.sin_family = AF_INET;
    if (inet_pton(AF_INET, ipAddress, &remoteAddr.sin_addr) <= 0) {
        printf("잘못된 IP 주소 형식입니다: %s\n", ipAddress);
        closesocket(sock);
        WSACleanup();
        return false; // 잘못된 IP 주소 형식일 경우 종료
    }
    remoteAddr.sin_port = htons(SERVERPORT);

    // 서버 연결
    if (connect(sock, (struct sockaddr*)&remoteAddr, sizeof(remoteAddr)) == SOCKET_ERROR) {
        printf("서버 연결 실패\n");
        closesocket(sock);
        WSACleanup();
        return false; // 서버 연결 실패 시 종료
    }

    int id;
    retval = recv(sock, (char*)&id, sizeof(id), 0); // ID 수신
    if (retval == SOCKET_ERROR) {
        printf("데이터 수신 실패\n");
        closesocket(sock);
        WSACleanup();
        return false; // 데이터 수신 실패 시 종료
    }

    SetID(id);
    return true; // 로그인 성공
}

INT_PTR CALLBACK IpInputDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    // IP 주소를 저장할 버퍼
    char* ipAddressBuffer = reinterpret_cast<char*>(lParam);  // 전달받은 lParam을 char*로 캐스팅합니다.

    switch (message)
    {
    case WM_INITDIALOG:
        // 초기화 시 대화상자에 기본값 설정
        SetDlgItemTextA(hDlg, IDC_ID_EDIT, ipAddressBuffer);  // 대화상자에 기본 IP 주소 설정
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            // 사용자가 입력한 IP 주소 가져오기
            GetDlgItemTextA(hDlg, IDC_ID_EDIT, ipAddressBuffer, 16);  // 전달받은 lParam을 그대로 수정합니다.
            EndDialog(hDlg, IDOK);
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, IDCANCEL);
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}





