#include "FileLoad.h"
#include "LineData.h"
#include <commdlg.h>
#include <fstream>

void LoadDrawing(HWND hWnd)
{
    OPENFILENAME ofn;
    WCHAR fileName[MAX_PATH] = L"";
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = L"모든 파일 (*.*)\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn)) {
        std::ifstream file(fileName, std::ios::binary);
        if (file.is_open()) {
            size_t count;
            file.read(reinterpret_cast<char*>(&count), sizeof(count));
            g_lines.resize(count);
            file.read(reinterpret_cast<char*>(g_lines.data()), sizeof(LineSegment) * count);
            file.close();

            InvalidateRect(hWnd, NULL, TRUE); // 화면 갱신
        }
    }
}
