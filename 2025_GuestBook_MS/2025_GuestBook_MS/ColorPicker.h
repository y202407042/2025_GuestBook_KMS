#pragma once
#include <windows.h>
#include <commdlg.h> // ChooseColor

class ColorPicker {
public:
    ColorPicker();
    COLORREF Show(HWND hwndParent); // ���� ���� ��ȭ���� ����
    COLORREF GetSelectedColor() const;

private:
    COLORREF customColors[16]; // ����� ���� �����
    COLORREF selectedColor;    // ���� ���� ����
};