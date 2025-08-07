// 2025_GuestBook_MS.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include "framework.h"
#include "2025_GuestBook_MS.h"
#include "Drawing.h"

/// 커밋 시 작성해야 하는 내용
#define MAX_LOADSTRING 100

// 전역 변수:
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

    // TODO: 여기에 코드를 입력합니다.
    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY2025GUESTBOOKMS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY2025GUESTBOOKMS));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
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

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY2025GUESTBOOKMS));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY2025GUESTBOOKMS);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

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

int g_x = 0, g_y = 0; // 마우스 이전 좌표값
bool abc = false;
RECT a;

/// 커밋 내용
PenTool tool;
bool isDrawing = false;
DrawingStorage g_drawingStorage;  // 그리기 저장소

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int lastX = -1, lastY = -1;  // 이전 좌표 저장용

    switch (message)
    {
    case WM_CREATE:
    {
        CreateToolButtons(hWnd);
        break;
    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case ID_BTN_PEN:
        {
            tool.SetTool(ToolType::Pen);
            lastX = lastY = -1;  // 툴 변경 시 좌표 초기화
            break;
        }
        case ID_BTN_SPRAY:
        {
            tool.SetTool(ToolType::Spray);
            lastX = lastY = -1;  // 툴 변경 시 좌표 초기화
            break;
        }
        case ID_BTN_BRUSH:
        {
            tool.SetTool(ToolType::Brush);
            lastX = lastY = -1;  // 툴 변경 시 좌표 초기화
            break;
        }
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    }
    case WM_LBUTTONDOWN: // 마우스 클릭으로 선 그리기
    {
        isDrawing = true;
        lastX = LOWORD(lParam);
        lastY = HIWORD(lParam);
        break;
    }
    case WM_LBUTTONUP:
    {
        isDrawing = false;
        tool.ResetPrevPoint();
        lastX = lastY = -1;
        break;
    }
    // 창 움직이면 사라지는데 vector 값으로 저장하고 그리면 됨
    case WM_MOUSEMOVE:
    {
        if (!isDrawing) break;

        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        HDC hdc = GetDC(hWnd);

        // 현재 툴에 따라 명령 생성 및 저장
        switch (tool.GetCurrentTool()) {
        case ToolType::Pen:
            if (lastX != -1 && lastY != -1) {
                auto penCmd = std::make_unique<PenCommand>(lastX, lastY, x, y);
                penCmd->Execute(hdc);  // 즉시 그리기
                g_drawingStorage.AddCommand(std::move(penCmd));  // 저장
            }
            break;
        case ToolType::Spray:
        {
            auto sprayCmd = std::make_unique<SprayCommand>(x, y);
            sprayCmd->Execute(hdc);  // 즉시 그리기
            g_drawingStorage.AddCommand(std::move(sprayCmd));  // 저장
            break;
        }
        case ToolType::Brush:
        {
            auto brushCmd = std::make_unique<BrushCommand>(x, y);
            brushCmd->Execute(hdc);  // 즉시 그리기
            g_drawingStorage.AddCommand(std::move(brushCmd));  // 저장
            break;
        }
        }

        lastX = x;
        lastY = y;
        ReleaseDC(hWnd, hdc);
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // 저장된 모든 그리기 명령 다시 실행
        g_drawingStorage.RedrawAll(hdc);

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