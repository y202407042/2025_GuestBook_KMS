#include "PenTool.h"
#include <random>

int prevX = 0, prevY = 0;
PenTool::PenTool() : currentTool(ToolType::Pen) {
	hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
}

PenTool::~PenTool() {
	DeleteObject(hPen);
}

void PenTool::SetTool(ToolType tool) {
	if (currentTool == tool)
		return;  // ���� ���̸� �׳� return

	currentTool = tool;

	// �� ���� �� ���� ��ǥ ���� �ʱ�ȭ
	hasPrevPoint = false;  // �� �� �߰�!

	// ���� �� ����
	if (hPen) {
		DeleteObject(hPen);
		hPen = NULL;
	}
}

void PenTool::Draw(HDC hdc, int x, int y) {

	switch (currentTool) {
	case ToolType::Pen:
		//CreatePen(PS_SOLID,3,RGB(0,0,0))
		SelectObject(hdc, hPen);
		// ó�� �׸� ���� prevX, prevY�� ������ �ϰ� ���� �׸��� ����
		if (!hasPrevPoint) {
			prevX = x;
			prevY = y;
			hasPrevPoint = true;
		}

		// ���� ��ǥ���� �� ��ǥ�� �� �׸���
		if (hasPrevPoint)
		{
			MoveToEx(hdc, prevX, prevY, NULL);
			LineTo(hdc, x, y);
		}
		prevX = x;
		prevY = y;
		break;

	case ToolType::Spray:
	{
		for (int i = 0; i < 20; ++i) {
			int offsetX = rand() % 11 - 5;
			int offsetY = rand() % 11 - 5;
			SetPixel(hdc, x + offsetX, y + offsetY, RGB(0, 0, 0));  // �������̴� �����ϰ� �� ���
		}
		break;
	}

	case ToolType::Brush:
		Ellipse(hdc, x - 5, y - 5, x + 5, y + 5);
		break;
	}
}
void PenTool::ResetPrevPoint() {
	hasPrevPoint = false;  // �� �Լ��� ����
}
