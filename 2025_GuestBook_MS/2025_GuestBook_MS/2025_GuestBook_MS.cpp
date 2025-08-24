// GuestPaint.cpp : 애플리케이션에 대한 진입점을 정의합니다.

#include "framework.h"
#include "2025_GuestBook_MS.h"

#define MAX_LOADSTRING 100

// 전역 변수
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
HWND hBtnPen, hBtnSpray, hBtnColor, hBtnSave, hBtnLoad, hBtnReplay;

// 전역 그리기 상태
bool isDrawing = false;
int lastX = 0, lastY = 0;

// 캔버스 영역 정의
RECT g_logoRect = { 0, 0, 800, 40 };
RECT g_toolbarRect = { 0, 40, 800, 80 };
RECT g_canvasRect = { 20, 100, 780, 580 };

// 함수 선언
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

// WinMain 함수
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_2025_GUESTBOOK_MS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
        return FALSE;

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

//
// 윈도우 클래스 등록
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    return RegisterClassExW(&wcex);
}

//
// 윈도우 생성
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 820, 640, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
        return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
// 메시지 처리 함수
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_LBUTTONDOWN:
        isDrawing = true;
        lastX = LOWORD(lParam);
        lastY = HIWORD(lParam);
        SetCapture(hWnd); // 마우스 캡처 시작
        break;

    case WM_LBUTTONUP:
        isDrawing = false;
        ReleaseCapture(); // 마우스 캡처 해제
        break;

    case WM_CAPTURECHANGED:
        isDrawing = false;  // 마우스가 창 밖으로 나간 후 버튼이 떨어졌을 때도 그리기 중지
        break;

    case WM_CREATE: // 버튼 코드
    {
        int left = 20;
        int top = 50;
        int btnWidth = 100;
        int btnHeight = 30;
        int gap = 10;

        hBtnPen = CreateWindowW(L"BUTTON", L"펜",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            left, top, btnWidth, btnHeight,
            hWnd, (HMENU)IDC_BTN_PEN, hInst, NULL);

        hBtnSpray = CreateWindowW(L"BUTTON", L"스프레이",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            left += btnWidth + gap, top, btnWidth, btnHeight,
            hWnd, (HMENU)IDC_BTN_SPRAY, hInst, NULL);

        hBtnColor = CreateWindowW(L"BUTTON", L"색상",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            left += btnWidth + gap, top, btnWidth, btnHeight,
            hWnd, (HMENU)IDC_BTN_COLOR, hInst, NULL);

        hBtnSave = CreateWindowW(L"BUTTON", L"저장",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            left += btnWidth + gap, top, btnWidth, btnHeight,
            hWnd, (HMENU)IDC_BTN_SAVE, hInst, NULL);

        hBtnLoad = CreateWindowW(L"BUTTON", L"불러오기",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            left += btnWidth + gap, top, btnWidth, btnHeight,
            hWnd, (HMENU)IDC_BTN_LOAD, hInst, NULL);

        hBtnReplay = CreateWindowW(L"BUTTON", L"리플레이",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            left += btnWidth + gap, top, btnWidth, btnHeight,
            hWnd, (HMENU)IDC_BTN_REPLAY, hInst, NULL);

        break;
    }

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDC_BTN_PEN:
            MessageBox(hWnd, L"펜 버튼 클릭", L"툴바", MB_OK); // 이 코드 지우고 기능 추가
            break;
        case IDC_BTN_SPRAY:
            MessageBox(hWnd, L"스프레이 버튼 클릭", L"툴바", MB_YESNO);
            break;
        case IDC_BTN_COLOR:
            MessageBox(hWnd, L"색상 버튼 클릭", L"툴바", MB_OK);
            break;
        case IDC_BTN_SAVE:
            MessageBox(hWnd, L"저장", L"툴바", MB_OK);
            break;
        case IDC_BTN_LOAD:
            MessageBox(hWnd, L"불러오기 버튼 클릭", L"툴바", MB_OKCANCEL);
            break;
        case IDC_BTN_REPLAY:
            MessageBox(hWnd, L"리플레이 버튼 클릭", L"툴바", MB_OK);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    }

    case WM_SIZE: // 사이즈 코드 올리기
    {
        int width = LOWORD(lParam);   // 클라이언트 영역 너비
        int height = HIWORD(lParam);  // 클라이언트 영역 높이

        // 창 높이 비율로 로고 높이 계산 (8%)
        int logoHeight = static_cast<int>(height * 0.08);

        // 최소·최대 제한 (너무 작거나 커지지 않게)
        if (logoHeight < 60) logoHeight = 60;
        if (logoHeight > 80) logoHeight = 80;

        // 로고 영역: 상단 고정, 높이 60
        g_logoRect.left = 0;
        g_logoRect.top = 0;
        g_logoRect.right = width;
        g_logoRect.bottom = logoHeight;

        // 툴바 영역: 그 아래, 높이 40
        g_toolbarRect.left = 0;
        g_toolbarRect.top = g_logoRect.bottom;
        g_toolbarRect.right = width;
        g_toolbarRect.bottom = g_toolbarRect.top + 40;

        // 캔버스 영역: 그 아래, 남은 전체
        g_canvasRect.left = 20;
        g_canvasRect.top = g_toolbarRect.bottom + 20;
        g_canvasRect.right = width - 20;
        g_canvasRect.bottom = height - 20;

        // 버튼 재배치
        int btnCount = 6;
        int gap = 10;
        int btnWidth = (width - (gap * (btnCount + 1))) / btnCount;
        int btnHeight = g_toolbarRect.bottom - g_toolbarRect.top - 10;
        int btnTop = g_toolbarRect.top + 5;

        MoveWindow(hBtnPen, gap, btnTop, btnWidth, btnHeight, TRUE);
        MoveWindow(hBtnSpray, gap * 2 + btnWidth, btnTop, btnWidth, btnHeight, TRUE);
        MoveWindow(hBtnColor, gap * 3 + btnWidth * 2, btnTop, btnWidth, btnHeight, TRUE);
        MoveWindow(hBtnSave, gap * 4 + btnWidth * 3, btnTop, btnWidth, btnHeight, TRUE);
        MoveWindow(hBtnLoad, gap * 5 + btnWidth * 4, btnTop, btnWidth, btnHeight, TRUE);
        MoveWindow(hBtnReplay, gap * 6 + btnWidth * 5, btnTop, btnWidth, btnHeight, TRUE);

        InvalidateRect(hWnd, NULL, TRUE); // 창 전체 다시 그리기
        break;
    }

    case WM_MOUSEMOVE:
        if (isDrawing)
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);

            if (PtInRect(&g_canvasRect, { x, y }))
            {
                HDC hdc = GetDC(hWnd);
                MoveToEx(hdc, lastX, lastY, NULL);
                LineTo(hdc, x, y);
                ReleaseDC(hWnd, hdc);
            }

            lastX = x;
            lastY = y;
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // 로고 영역
        FillRect(hdc, &g_logoRect, (HBRUSH)(COLOR_BTNFACE + 1));
        DrawTextW(hdc, L"유한대 로고", -1, &g_logoRect,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        // 툴바 영역
        FillRect(hdc, &g_toolbarRect, (HBRUSH)(COLOR_BTNHIGHLIGHT + 1));
        //DrawTextW(hdc, L"펜  스프레이  색상  저장  불러오기  리플레이  지우기", -1,
            //&g_toolbarRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        // 캔버스 영역
        HBRUSH canvasBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
        // 캔버스 영역
        FillRect(hdc, &g_canvasRect, (HBRUSH)GetStockObject(WHITE_BRUSH));
        FrameRect(hdc, &g_canvasRect, (HBRUSH)(COLOR_WINDOW + 1));


        EndPaint(hWnd, &ps);
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);

        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
