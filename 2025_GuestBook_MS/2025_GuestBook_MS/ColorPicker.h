#pragma once
#include <windows.h>
#include <commdlg.h> // ChooseColor

class ColorPicker {
public:
    ColorPicker();
    COLORREF Show(HWND hwndParent); // 색상 선택 대화상자 실행
    COLORREF GetSelectedColor() const;

private:
    COLORREF customColors[16]; // 사용자 색상 저장용
    COLORREF selectedColor;    // 최종 선택 색상
};