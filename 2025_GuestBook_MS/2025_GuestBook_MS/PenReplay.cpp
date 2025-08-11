#include "PenReplay.h"

/// 리플레이를 위해 리플레이 벡터에 마우스 좌표값을 저장
void PenReplay::AddPoint(POINT pt)
{
	if (!IsReplaying())
	{
		if (original.empty() || original.back().x != pt.x || original.back().y != pt.y)
		{
			original.push_back(pt);
		}
	}
}

/// 리플레이 시작
void PenReplay::Start(HWND hWnd)
{
	/// 원본 좌표(original)이 비어있거나, 이미 리플레이 중이라면 작업 X
	if (original.empty() || IsReplaying())
	{
		return;
	}

	replayBuffer.clear();										/// 이전에 사용한 리플레이 버퍼 초기화
	isReplayingFlag = true;										/// 현재 리플레이 상태를 true로 변경
	timerId = SetTimer(hWnd, TIMER_ID, TIMER_INTERVAL, NULL);	/// 타이머 시작
}

/// 리플레이 중단
void PenReplay::Stop(HWND hWnd)
{
	if (timerId)												/// 타이머가 동작 중이라면 종료
	{
		KillTimer(hWnd, timerId);								/// 지정된 타이머 ID 중지
		timerId = 0;											/// 타이머 ID 초기화
	}

	isReplayingFlag = false;									/// 현재 리플레이 상태를 false로 변경
}


void PenReplay::HandleTimer(HWND hWnd)
{
	if (!original.empty())
	{
		replayBuffer.push_back(original.front());
		original.erase(original.begin());
		InvalidateRect(hWnd, NULL, TRUE);
	}
	else
	{
		Stop(hWnd);
	}
}

void PenReplay::Draw(HWND hWnd, HDC hdc)
{
	RECT rect;
	GetClientRect(hWnd, &rect);

	if (!memDC || !memBmp)
	{
		InitializeBuffer(hWnd, rect);
	}


	/// 배경 지우기
	FillRect(memDC, &rect, (HBRUSH)(COLOR_WINDOW + 1));

	/// 선 그리기
	const auto& points = IsReplaying() ? replayBuffer : original;
	if (points.size() >= 2) {
		MoveToEx(memDC, points[0].x, points[0].y, NULL);
		for (size_t i = 1; i < points.size(); ++i) {
			LineTo(memDC, points[i].x, points[i].y);
		}
	}

	// 백버퍼 -> 화면 복사
	BitBlt(hdc, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);
}

/// 윈도우 시작 시 한번만 호출
void PenReplay::InitializeBuffer(HWND hWnd, const RECT& RC)
{
	HDC hdc = GetDC(hWnd);

	/// 메모리 DC 생성
	memDC = CreateCompatibleDC(hdc);
	/// 메모리 DC 전용 그림판이라고 생각하면 됨
	memBmp = CreateCompatibleBitmap(hdc, RC.right - RC.left,RC.bottom - RC.top);
	/// memBmp의 값을 안전하게 보관, oldBmp에 추가 생성하여 관리
	oldBmp = (HBITMAP)SelectObject(memDC, memBmp);
	ReleaseDC(hWnd, hdc);

	/// 백버퍼 배경을 흰색으로 초기화
	HBRUSH brush = (HBRUSH)GetStockObject(WHITE_BRUSH);
	/// 흰색 브러쉬를 통해 rc 영역을 흰색으로 만듦
	FillRect(memDC, &RC, brush);
}

/// 사용했던 버퍼 모두 초기화
void PenReplay::CleanUpBuffer()
{
	if (memDC)
	{
		SelectObject(memDC, oldBmp);		/// 원래 비트맵으로 복원
		DeleteObject(memBmp);				/// 비트맵 자원 삭제
		DeleteDC(memDC);					/// 메모리 DC 삭제

		/// 포인터 모두 초기화
		memDC = nullptr;
		memBmp = nullptr;
		oldBmp = nullptr;

	}
}

