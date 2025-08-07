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
    bool hasPrevPoint = false;  // false로 초기화해야 함

public:
    PenTool();
    ~PenTool();
    void SetTool(ToolType tool);
    void Draw(HDC hdc, int x, int y);
    void ResetPrevPoint(); // 마우스 뗐을 때 호출

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
