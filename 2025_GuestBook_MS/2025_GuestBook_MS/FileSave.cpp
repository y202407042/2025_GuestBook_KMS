#include "FileSave.h"
#include "LineData.h"
#include <commdlg.h>
#include <fstream>

void SaveDrawing(HWND hWnd)
{
    OPENFILENAME ofn;
    WCHAR fileName[MAX_PATH] = L"";
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = L"모든 파일 (*.*)\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT;

    if (GetSaveFileName(&ofn)) {
        std::ofstream file(fileName, std::ios::binary);
        size_t count = g_lines.size();
        file.write(reinterpret_cast<char*>(&count), sizeof(count));
        file.write(reinterpret_cast<char*>(g_lines.data()), sizeof(LineSegment) * count);
        file.close();
    }
}