#include "main.h"
#include "GameFramework.h"
#include "Packet.h"
#define MAX_LOADSTRING 100
// test
// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

DWORD __stdcall SendData(LPVOID arg);
DWORD __stdcall ReceiveData(LPVOID arg);

CGameFramework GameFramework;
SOCKET sock;

HANDLE hSelectEvent;

SC_PlayersInfoPacket receivedPacket;


void ProcessInput();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SAMPLE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SAMPLE));

    MSG msg;


    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("윈속 초기화 실패");
        return 1;
    }

    // 소켓 생성
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("소켓 생성 실패");
        WSACleanup();
        return 1;
    }

    hSelectEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    // 서버 주소 설정
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr);
    serveraddr.sin_port = htons(SERVERPORT);

    // 서버 연결
    if (connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR) {
        printf("서버 연결 실패");
        closesocket(sock);
        WSACleanup();
        exit(0);
    }

    printf("서버 연결 성공");


    CreateThread(NULL, 0, SendData, NULL, 0, NULL);
    CreateThread(NULL, 0, ReceiveData, NULL, 0, NULL);

    CloseHandle(hSelectEvent);


    while (1)
    {
        if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) break;
            if (!::TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
        }
        else
        {
            ProcessInput();
            GameFramework.FrameAdvance();
        }
    }

    // GameFramework.OnDestroy();
    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SAMPLE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
   DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_BORDER;
   HWND hWnd = CreateWindow(szWindowClass, szTitle, dwStyle,
       0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, NULL, (HMENU)NULL, hInstance, NULL);

   if (!hWnd) return(FALSE);

   GameFramework.Initialize(hWnd, hInst);



   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

    switch (message)
    {
    case WM_SIZE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MOUSEMOVE:
    case WM_KEYDOWN:
    case WM_KEYUP:
        GameFramework.OnProcessingWindowMessage(hWnd, message, wParam, lParam);
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

#define _CRT_SECURE_NO_WARNINGS 

int keyData;

char GetPressedKeysAsString();
void ProcessInput()
{
    // 현재 눌린 키들을 문자열로 가져오기
    char pressedKeys = GetPressedKeysAsString();

    if (pressedKeys) {
        keyData = pressedKeys;
    }
}

char GetPressedKeys()
{
    char pressedKey = '0';

    // 모든 가상 키 코드(0x01부터 0xFE까지)를 반복
    for (int key = 0x01; key <= 0xFE; ++key) {
        // 키가 눌려 있는지 확인
        if (GetAsyncKeyState(key) & 0x8000) {
            pressedKey = key; // 눌린 키를 추가
        }
    }

    return pressedKey;
}

char GetPressedKeysAsString()
{
    // 눌려 있는 키 목록을 가져옴
    char pressedKeys = GetPressedKeys();

    if (!pressedKeys) pressedKeys = '0';

    return pressedKeys;
}

DWORD __stdcall SendData(LPVOID arg)
{
    int retval;

    //WaitForSingleObject(hSelectEvent, INFINITE);            // 서버 연결 전에 데이터 전송 막기
    int tempData = 0;

    while (1) {
        if (tempData != keyData) {
            tempData = keyData;
            retval = send(sock, (char*)&tempData, sizeof(tempData), 0);
            printf("보낸 데이터 : %d\r", tempData);
        }
    }

    return 0;
}


DWORD __stdcall ReceiveData(LPVOID arg)
{
    int retval;

    //WaitForSingleObject(hSelectEvent, INFINITE);            // 서버 연결 전에 데이터 전송 막기

    // TODO : 서버로부터 Player 데이터를 받아야 한다.
    while (1) {
        retval = recv(sock, (char*)&receivedPacket, sizeof(SC_PlayersInfoPacket), 0);
        if (retval > 0) {
            // 수신한 데이터로 처리      
            printf("Player 데이터 수신 성공: X=%d, Y=%d, State = %d\n",        // DEBUG
                receivedPacket.player1.x,
                receivedPacket.player1.y,
                receivedPacket.player1.GetState());
        }
    }


    
    return 0;
}
