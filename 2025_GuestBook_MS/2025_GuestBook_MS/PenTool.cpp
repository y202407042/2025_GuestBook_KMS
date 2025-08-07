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
		return;  // 같은 툴이면 그냥 return

	currentTool = tool;

	// 툴 변경 시 이전 좌표 정보 초기화
	hasPrevPoint = false;  // 이 줄 추가!

	// 기존 펜 삭제
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
		// 처음 그릴 때는 prevX, prevY를 설정만 하고 선을 그리지 않음
		if (!hasPrevPoint) {
			prevX = x;
			prevY = y;
			hasPrevPoint = true;
		}

		// 기존 좌표에서 새 좌표로 선 그리기
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
			SetPixel(hdc, x + offsetX, y + offsetY, RGB(0, 0, 0));  // 스프레이는 랜덤하게 점 찍기
		}
		break;
	}

	case ToolType::Brush:
		Ellipse(hdc, x - 5, y - 5, x + 5, y + 5);
		break;
	}
}
void PenTool::ResetPrevPoint() {
	hasPrevPoint = false;  // 이 함수도 수정
}
