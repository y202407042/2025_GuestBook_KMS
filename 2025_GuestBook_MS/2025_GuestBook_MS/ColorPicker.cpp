#include "ColorPicker.h"

ColorPicker::ColorPicker() : selectedColor(RGB(0, 0, 0)) {
    // 사용자 색상 초기화 (기본 흰색으로 설정)
    for (int i = 0; i < 16; ++i) {
        customColors[i] = RGB(255, 255, 255);
    }
}

COLORREF ColorPicker::Show(HWND hwndParent) {
    CHOOSECOLOR cc = { 0 };
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = hwndParent;
    cc.rgbResult = selectedColor;
    cc.lpCustColors = customColors;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;

    if (ChooseColor(&cc)) {
        selectedColor = cc.rgbResult;
    }

    return selectedColor;
}

COLORREF ColorPicker::GetSelectedColor() const {
    return selectedColor;
}