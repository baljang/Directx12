// Client.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "Client.h"
#include "Game.h"

#define MAX_LOADSTRING 100

// Global Variables:
WindowInfo GWindowInfo;    // ���⿡ �̷����� ������ ����ٰž�. 

HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ���Ե� �Լ��� ������ �����մϴ�:
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
    unique_ptr<Game> game = make_unique<Game>();    // Game ����
    // unique_ptr�� pch.h�� �־��� <memory>�� ����ֱ⿡ Ȱ�� ����
    game->Init(GWindowInfo);  // �ʱ�ȭ 


    // Main message loop:
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))      // PeekMessage�� �޽��� �ִ��� �������� ��¦ ���� �� �ִ�. �޽��� ������ �׳� �״�� �����ų �� �־�. 
        // ���ڸ� nullptr�� �־�θ� ���� ������� ���õ� ��� �޽����� �ްԵȴ�. 0, 0���� �޾��ָ� ��� �޽��� �޾��ִ°�. 
        // PM_REMOVE�� �����쿡�� Ű���� �����ų� ���콺 �����̰ų� �ϴ� �̺�Ʈ�� �߻���Ű�� �޽����� ó���ϴµ� �޽���ť�� ���������� �־��ְ� ��. 
        // �װ� �ϳ��� ������ ����ϴ� ����. &msg�� ���캸�µ� PM_RMOVE���ָ� ť���� �����ִ°� �ȴ�. ���������� ���̻� �ʿ� ���� �Ǵϱ� remove ���شٰ� ���� ��. ���ϰ� ���� �ʾƵ� ��.
        {
            if (msg.message == WM_QUIT) // �޽����� WM_QUIT�̸� break�� while�� ���������� ����. ���α׷� ������ �� ���� ����ϴ� �ڵ�. 0x0012�� message�� �־��ָ� ���������� �ǹ�. 
                break; 

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) // �޽����� ó�����ִ� �κ� if(PeekMessage())�� �ۿ� �־��µ� �ȿ� �־���. 
                                                                    // ���� ���Դٴ� �� PeekMessage�� true�� messageó������. 
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        // TODO 
        // PeekMessage�� true�� �ƴϰ� ����� ������ ��. ���ӷ����� �߰����ָ� ��. 
        game->Update(); // ������ �ѹ� �� �� ���� Update�Լ��� �����Ұž�. � �����̰� ���� ������� �Ǿ� �־�. 
        // �� ƽ���� Update�� ȣ���ϴ� ������� ���������� �⺻������ �̷� ������� ������ ���ư��ٰ� ���� ��. 
    }

    return (int) msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  �뵵: â Ŭ������ ����մϴ�.
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
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   �뵵: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   �ּ�:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
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

   // hWnd ��ü�� ���ÿ��ٰ� Ȱ���ߴٰ� �����ִµ� �̰� ���� ������ �������ָ� ������ ��.
   GWindowInfo.hwnd = hWnd;


   return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �뵵: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���ø����̼� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
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

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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
