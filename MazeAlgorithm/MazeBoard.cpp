#include "MazeBoard.h"

void MazeBoard::Init(int size) {
    if (size % 2 == 0) size++; // 짝수 사이즈 보정
    m_size = size;
    startX = 1; startY = 1;
    goalX = m_size - 2; goalY = m_size - 2;
}

void MazeBoard::ClearBoard(bool fillWithWall) {
    for (int y = 0; y < m_size; y++) {
        for (int x = 0; x < m_size; x++) {
            if (fillWithWall) tile[y][x] = Wall;
            else tile[y][x] = (x % 2 == 0 || y % 2 == 0) ? Wall : Empty;
        }
    }
}

bool MazeBoard::IsValid(int x, int y) {
    if (x < 0 || x >= m_size || y < 0 || y >= m_size) return false;
    return tile[y][x] != Wall;
}

void MazeBoard::Render() {
    // 기존 Render 코드 그대로 이동 (단, tile[y][x] 사용)
    for (int y = 0; y < m_size; y++) {
        for (int x = 0; x < m_size; x++) {
            if (x == startX && y == startY) cout << "\033[42m  \033[0m";
            else if (x == goalX && y == goalY) cout << "\033[41m  \033[0m";
            else {
                switch (tile[y][x]) {
                case Wall: cout << "\033[47m  \033[0m"; break;
                case Path: cout << "\033[44m  \033[0m"; break;
                case Loot: cout << "\033[43m  \033[0m"; break;
                case Back: cout << "\033[41m  \033[0m"; break;
                case Scan: cout << "\033[42m  \033[0m"; break;
                default:   cout << "\033[40m  \033[0m"; break;
                }
            }
        }
        cout << endl;
    }
}