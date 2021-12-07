// Client.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "Client.h"
#include "Game.h"

#define MAX_LOADSTRING 100

// Global Variables:
WindowInfo GWindowInfo;    // 여기에 이런저런 정보를 담아줄거야. 

HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

    MSG msg;

    GWindowInfo.width = 800;
    GWindowInfo.height = 600;
    GWindowInfo.windowed = true;

    //Game* game = new Game();    
    unique_ptr<Game> game = make_unique<Game>();    // Game 생성
    // unique_ptr은 pch.h에 넣어준 <memory>에 들어있기에 활용 가능
    game->Init(GWindowInfo);  // 초기화 


    // Main message loop:
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))      // PeekMessage는 메시지 있는지 없는지만 살짝 엿볼 수 있다. 메시지 없으면 그냥 그대로 실행시킬 수 있어. 
        // 인자를 nullptr을 넣어두면 현재 스레드와 관련된 모든 메시지를 받게된다. 0, 0으로 받아주면 모든 메시지 받아주는거. 
        // PM_REMOVE는 윈도우에서 키보드 누르거나 마우스 움직이거나 하는 이벤트를 발생시키면 메시지로 처리하는데 메시지큐에 순차적으로 넣어주게 됨. 
        // 그걸 하나씩 꺼내서 사용하는 개념. &msg로 살펴보는데 PM_RMOVE해주면 큐에서 꺼내주는게 된다. 꺼내왔으면 더이상 필요 없게 되니까 remove 해준다고 보면 됨. 상세하게 알지 않아도 됨.
        {
            if (msg.message == WM_QUIT) // 메시지가 WM_QUIT이면 break로 while문 빠져나가서 종료. 프로그램 종료할 때 자주 사용하는 코드. 0x0012를 message로 넣어주면 종료해줘라는 의미. 
                break; 

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) // 메시지를 처리해주는 부분 if(PeekMessage())문 밖에 있었는데 안에 넣어줌. 
                                                                    // 여기 들어왔다는 건 PeekMessage가 true니 message처리해줌. 
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        // TODO 
        // PeekMessage가 true건 아니건 여기로 들어오게 됨. 게임로직을 추가해주면 됨. 
        game->Update(); // 루프를 한번 돌 때 마다 Update함수를 실행할거야. 어떤 게임이건 같은 방식으로 되어 있어. 
        // 매 틱마다 Update를 호출하는 방식으로 무식하지만 기본적으로 이런 방식으로 게임이 돌아간다고 보면 됨. 
    }

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName = nullptr; //  MAKEINTRESOURCEW(IDC_CLIENT);
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
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   // hWnd 가체가 스택에다가 활용했다가 끝내주는데 이걸 전역 변수에 저장해주면 마무리 됨.
   GWindowInfo.hwnd = hWnd;


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
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
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
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
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
