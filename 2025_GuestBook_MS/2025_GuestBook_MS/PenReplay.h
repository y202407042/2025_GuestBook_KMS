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
	HDC memDC = nullptr;		/// 메모리 DC
	HBITMAP memBmp = nullptr;	/// 백버퍼 비트맵
	HBITMAP oldBmp = nullptr;	/// 복원용 비트맵

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




