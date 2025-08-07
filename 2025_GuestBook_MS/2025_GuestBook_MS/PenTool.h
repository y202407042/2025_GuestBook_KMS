#pragma once
#include <windows.h>
#include <vector>

enum class ToolType {
    Pen,
    Spray,
    Brush
};

class PenTool {
private:
    ToolType currentTool;
    HPEN hPen;
    int prevX, prevY;
    bool hasPrevPoint = false;  // false�� �ʱ�ȭ�ؾ� ��

public:
    PenTool();
    ~PenTool();
    void SetTool(ToolType tool);
    void Draw(HDC hdc, int x, int y);
    void ResetPrevPoint(); // ���콺 ���� �� ȣ��

    ToolType GetCurrentTool() const {
        return currentTool;
    }
    bool GetPrevPoint(int& x, int& y) const {
        if (hasPrevPoint) {
            x = prevX;
            y = prevY;
            return true;
        }
        return false;
    }

    void UpdatePrevPoint(int x, int y) {
        prevX = x;
        prevY = y;
        hasPrevPoint = true;
    }
};
