#pragma once
#include <iostream>
#include <vector>
#include <windows.h>
using namespace std;

void SetCursorPosition(int x, int y);
void HideCursor();

class Board {
public:
    enum TileType {
        Empty,
        Back,
        Wall,
        Path,
        Scan,
        Loot
    };
private:
    TileType tile[50][50];
    int m_size;
    int m_startX, m_startY;
    int m_goalX, m_goalY;

    struct Node {
        int m_x, m_y;
        int m_g, m_h, m_f;
        Node* m_parent;
        Node(int x, int y, int g, int h, Node* parent = nullptr);
        bool operator>(const Node& other) const;
    };

    int CalculateH(int x1, int y1, int x2, int y2);
    bool IsValid(int x, int y);
    void GenerateBinaryTree(bool showProcess);
    void GenerateGrowingBinaryTree(bool showProcess);
    void GenerateRecursiveBacktracking(bool showProcess);
    void GenerateHuntandKill(bool showProcess);

public:
    void Initialize(int boardSize, int algorithm, bool showProcess = false);
    void PathFindingChoose(int mapsize);
    void Render();
    void Breadthfirstsearch(int sX, int sY, int gX, int gY);
    void Depthfirstsearch(int sX, int sY, int gX, int gY);
    void Algorithm(int choice);
};