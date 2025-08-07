#pragma once
#pragma once
#include <windows.h>
#include <vector>
#include <memory>

// 그리기 명령의 기본 클래스
class DrawCommand {
public:
    virtual ~DrawCommand() = default;
    virtual void Execute(HDC hdc) = 0;
    virtual DrawCommand* Clone() = 0;
};

// 펜 그리기 명령
class PenCommand : public DrawCommand {
private:
    int startX, startY, endX, endY;
    COLORREF color;
    int width;

public:
    PenCommand(int sx, int sy, int ex, int ey, COLORREF c = RGB(0, 0, 0), int w = 2)
        : startX(sx), startY(sy), endX(ex), endY(ey), color(c), width(w) {}

    void Execute(HDC hdc) override {
        HPEN hPen = CreatePen(PS_SOLID, width, color);
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        
        MoveToEx(hdc, startX, startY, NULL);
        LineTo(hdc, endX, endY);
        
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
    }

    DrawCommand* Clone() override {
        return new PenCommand(startX, startY, endX, endY, color, width);
    }
};

// 스프레이 그리기 명령
class SprayCommand : public DrawCommand {
private:
    int centerX, centerY;
    COLORREF color;
    std::vector<POINT> points;

public:
    SprayCommand(int x, int y, COLORREF c = RGB(0, 0, 0)) 
        : centerX(x), centerY(y), color(c) {
        // 스프레이 점들을 미리 생성
        for (int i = 0; i < 20; ++i) {
            int offsetX = rand() % 11 - 5;
            int offsetY = rand() % 11 - 5;
            points.push_back({x + offsetX, y + offsetY});
        }
    }

    void Execute(HDC hdc) override {
        for (const auto& point : points) {
            SetPixel(hdc, point.x, point.y, color);
        }
    }

    DrawCommand* Clone() override {
        SprayCommand* cmd = new SprayCommand(centerX, centerY, color);
        cmd->points = this->points; // 같은 점들 복사
        return cmd;
    }
};

// 브러시 그리기 명령
class BrushCommand : public DrawCommand {
private:
    int centerX, centerY;
    int radius;
    COLORREF color;

public:
    BrushCommand(int x, int y, int r = 5, COLORREF c = RGB(0, 0, 0))
        : centerX(x), centerY(y), radius(r), color(c) {}

    void Execute(HDC hdc) override {
        HBRUSH hBrush = CreateSolidBrush(color);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
        
        Ellipse(hdc, centerX - radius, centerY - radius, 
                centerX + radius, centerY + radius);
        
        SelectObject(hdc, hOldBrush);
        DeleteObject(hBrush);
    }

    DrawCommand* Clone() override {
        return new BrushCommand(centerX, centerY, radius, color);
    }
};

// 그리기 저장소 클래스
class DrawingStorage {
private:
    std::vector<std::unique_ptr<DrawCommand>> commands;

public:
    ~DrawingStorage() {
        Clear();
    }

    // 명령 추가
    void AddCommand(std::unique_ptr<DrawCommand> command) {
        commands.push_back(std::move(command));
    }

    // 모든 명령 실행 (다시 그리기)
    void RedrawAll(HDC hdc) {
        for (const auto& command : commands) {
            command->Execute(hdc);
        }
    }

    // 모든 명령 삭제
    void Clear() {
        commands.clear();
    }

    // 마지막 명령 취소
    void Undo() {
        if (!commands.empty()) {
            commands.pop_back();
        }
    }

    // 저장된 명령 개수
    size_t GetCommandCount() const {
        return commands.size();
    }
};