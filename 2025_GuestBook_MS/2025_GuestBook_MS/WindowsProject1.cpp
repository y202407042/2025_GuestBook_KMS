// GuestPaint.cpp : 애플리케이션에 대한 진입점을 정의합니다.

#include "framework.h"
#include "WindowsProject1.h"
#include <windows.h>
#include "ColorPicker.h"  // 색상 선택기 클래스

#define MAX_LOADSTRING 100
#define ID_COLOR_BUTTON 1001 // 버튼 컨트롤 ID

// 전역 변수
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
ColorPicker g_colorPicker;                    // 전역 ColorPicker 객체
COLORREF g_penColor = RGB(0, 0, 0);           // 선택된 펜 색상



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
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
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

    case WM_SIZE:
    {
        int width = LOWORD(lParam);   // 클라이언트 영역 너비
        int height = HIWORD(lParam);  // 클라이언트 영역 높이

        // 로고 영역: 상단 고정, 높이 40
        g_logoRect.left = 0;
        g_logoRect.top = 0;
        g_logoRect.right = width;
        g_logoRect.bottom = 40;

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

        InvalidateRect(hWnd, NULL, TRUE); // 창 전체 다시 그리기
        break;
    }

    // 색상 선택 버튼 생성
    case WM_CREATE:
        CreateWindowW(L"button", L"색상 선택",
            WS_VISIBLE | WS_CHILD,
            600, 50, 100, 30,   // 툴바 위치 기준 위치 조절
            hWnd, (HMENU)ID_COLOR_BUTTON, hInst, nullptr);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_COLOR_BUTTON) {
            g_penColor = g_colorPicker.Show(hWnd);  // 색상 선택
        }
        break;

    case WM_MOUSEMOVE:
        if (isDrawing)
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);

            if (PtInRect(&g_canvasRect, { x, y }))
            {
                HDC hdc = GetDC(hWnd);
                HPEN hPen = CreatePen(PS_SOLID, 2, g_penColor);  // 선택된 색상 사용
                HGDIOBJ oldPen = SelectObject(hdc, hPen);

                MoveToEx(hdc, lastX, lastY, NULL);
                LineTo(hdc, x, y);

                SelectObject(hdc, oldPen);
                DeleteObject(hPen);
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
        DrawTextW(hdc, L"펜  스프레이  색상  저장  불러오기  리플레이", -1,
            &g_toolbarRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

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