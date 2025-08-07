#include "ToolButton.h"

void CreateToolButtons(HWND hWnd) {
    CreateWindow(TEXT("BUTTON"), TEXT("Pen"),
        WS_VISIBLE | WS_CHILD,
        10, 10, 80, 30,
        hWnd, (HMENU)ID_BTN_PEN,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);

    CreateWindow(TEXT("BUTTON"), TEXT("Spray"),
        WS_VISIBLE | WS_CHILD,
        100, 10, 80, 30,
        hWnd, (HMENU)ID_BTN_SPRAY,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);

    CreateWindow(TEXT("BUTTON"), TEXT("Brush"),
        WS_VISIBLE | WS_CHILD,
        190, 10, 80, 30,
        hWnd, (HMENU)ID_BTN_BRUSH,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);
}
