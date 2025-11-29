#pragma once
#include "Common.h"

class MazeBoard {
private:
    int m_size;
    TileType tile[50][50];

public:
    int startX, startY;
    int goalX, goalY;

    void Init(int size);
    void Render();

    // 외부(Generator, Solver)에서 타일을 수정할 수 있게 해주는 함수
    void SetTile(int x, int y, TileType type) { tile[y][x] = type; }
    TileType GetTile(int x, int y) const { return tile[y][x]; }
    int GetSize() const { return m_size; }

    bool IsValid(int x, int y);
    void ClearBoard(bool fillWithWall);
};