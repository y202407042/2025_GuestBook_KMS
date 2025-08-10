#pragma once
#include <vector>

struct LineSegment {
    int x1, y1;
    int x2, y2;
};

extern std::vector<LineSegment> g_lines;
