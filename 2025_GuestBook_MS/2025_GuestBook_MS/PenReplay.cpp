#include "PenReplay.h"

/// ���÷��̸� ���� ���÷��� ���Ϳ� ���콺 ��ǥ���� ����
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

/// ���÷��� ����
void PenReplay::Start(HWND hWnd)
{
	/// ���� ��ǥ(original)�� ����ְų�, �̹� ���÷��� ���̶�� �۾� X
	if (original.empty() || IsReplaying())
	{
		return;
	}

	replayBuffer.clear();										/// ������ ����� ���÷��� ���� �ʱ�ȭ
	isReplayingFlag = true;										/// ���� ���÷��� ���¸� true�� ����
	timerId = SetTimer(hWnd, TIMER_ID, TIMER_INTERVAL, NULL);	/// Ÿ�̸� ����
}

/// ���÷��� �ߴ�
void PenReplay::Stop(HWND hWnd)
{
	if (timerId)												/// Ÿ�̸Ӱ� ���� ���̶�� ����
	{
		KillTimer(hWnd, timerId);								/// ������ Ÿ�̸� ID ����
		timerId = 0;											/// Ÿ�̸� ID �ʱ�ȭ
	}

	isReplayingFlag = false;									/// ���� ���÷��� ���¸� false�� ����
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


	/// ��� �����
	FillRect(memDC, &rect, (HBRUSH)(COLOR_WINDOW + 1));

	/// �� �׸���
	const auto& points = IsReplaying() ? replayBuffer : original;
	if (points.size() >= 2) {
		MoveToEx(memDC, points[0].x, points[0].y, NULL);
		for (size_t i = 1; i < points.size(); ++i) {
			LineTo(memDC, points[i].x, points[i].y);
		}
	}

	// ����� -> ȭ�� ����
	BitBlt(hdc, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);
}

/// ������ ���� �� �ѹ��� ȣ��
void PenReplay::InitializeBuffer(HWND hWnd, const RECT& RC)
{
	HDC hdc = GetDC(hWnd);

	/// �޸� DC ����
	memDC = CreateCompatibleDC(hdc);
	/// �޸� DC ���� �׸����̶�� �����ϸ� ��
	memBmp = CreateCompatibleBitmap(hdc, RC.right - RC.left,RC.bottom - RC.top);
	/// memBmp�� ���� �����ϰ� ����, oldBmp�� �߰� �����Ͽ� ����
	oldBmp = (HBITMAP)SelectObject(memDC, memBmp);
	ReleaseDC(hWnd, hdc);

	/// ����� ����� ������� �ʱ�ȭ
	HBRUSH brush = (HBRUSH)GetStockObject(WHITE_BRUSH);
	/// ��� �귯���� ���� rc ������ ������� ����
	FillRect(memDC, &RC, brush);
}

/// ����ߴ� ���� ��� �ʱ�ȭ
void PenReplay::CleanUpBuffer()
{
	if (memDC)
	{
		SelectObject(memDC, oldBmp);		/// ���� ��Ʈ������ ����
		DeleteObject(memBmp);				/// ��Ʈ�� �ڿ� ����
		DeleteDC(memDC);					/// �޸� DC ����

		/// ������ ��� �ʱ�ȭ
		memDC = nullptr;
		memBmp = nullptr;
		oldBmp = nullptr;

	}
}

