#pragma once
#pragma once
#include <windows.h>
#include <vector>
#include <memory>

// �׸��� ����� �⺻ Ŭ����
class DrawCommand {
public:
    virtual ~DrawCommand() = default;
    virtual void Execute(HDC hdc) = 0;
    virtual DrawCommand* Clone() = 0;
};

// �� �׸��� ���
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

// �������� �׸��� ���
class SprayCommand : public DrawCommand {
private:
    int centerX, centerY;
    COLORREF color;
    std::vector<POINT> points;

public:
    SprayCommand(int x, int y, COLORREF c = RGB(0, 0, 0)) 
        : centerX(x), centerY(y), color(c) {
        // �������� ������ �̸� ����
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
        cmd->points = this->points; // ���� ���� ����
        return cmd;
    }
};

// �귯�� �׸��� ���
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

// �׸��� ����� Ŭ����
class DrawingStorage {
private:
    std::vector<std::unique_ptr<DrawCommand>> commands;

public:
    ~DrawingStorage() {
        Clear();
    }

    // ��� �߰�
    void AddCommand(std::unique_ptr<DrawCommand> command) {
        commands.push_back(std::move(command));
    }

    // ��� ��� ���� (�ٽ� �׸���)
    void RedrawAll(HDC hdc) {
        for (const auto& command : commands) {
            command->Execute(hdc);
        }
    }

    // ��� ��� ����
    void Clear() {
        commands.clear();
    }

    // ������ ��� ���
    void Undo() {
        if (!commands.empty()) {
            commands.pop_back();
        }
    }

    // ����� ��� ����
    size_t GetCommandCount() const {
        return commands.size();
    }
};