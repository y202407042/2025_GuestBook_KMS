#pragma once
#include <vector>
#include <windows.h>
class PenReplay
{
private:
	std::vector<POINT> original;
	std::vector<POINT> replayBuffer;
	UINT_PTR timerId = 0;
	static const int TIMER_INTERVAL = 10;
	static const int TIMER_ID = 1001;
	bool isReplayingFlag = false;
	HDC memDC = nullptr;		/// �޸� DC
	HBITMAP memBmp = nullptr;	/// ����� ��Ʈ��
	HBITMAP oldBmp = nullptr;	/// ������ ��Ʈ��

public:
	void AddPoint(POINT pt);
	void Start(HWND hWnd);
	void Stop(HWND hWnd);
	void HandleTimer(HWND hWnd);
	void Draw(HWND hWnd, HDC hdc);
	bool IsReplaying() const { return isReplayingFlag; }
	void InitializeBuffer(HWND hWnd, const RECT& RC);
	void CleanUpBuffer();
};




