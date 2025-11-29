#pragma once
#include <windows.h>
#include <iostream>
#include <vector>

using namespace std;

// 타일 타입은 보드, 생성기, 솔버 모두가 알아야 하므로 여기 둡니다.
enum TileType {
    Empty,
    Back,
    Wall,
    Path,
    Scan,
    Loot
};

// 콘솔 관련 함수들은 전역함수 혹은 네임스페이스로 뺍니다.
inline void SetCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

inline void HideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}
