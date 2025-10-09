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
        Wall,
        Path
    };
private:
    TileType tile[50][50];
    int size;
    int startX, startY;
    int goalX, goalY;

    struct Node {
        int x, y;
        int g, h, f;
        Node* parent;
        Node(int x, int y, int g, int h, Node* parent = nullptr);
        bool operator>(const Node& other) const;
    };

    int CalculateH(int x1, int y1, int x2, int y2);
    bool IsValid(int x, int y);
    void GenerateBinaryTree(bool showProcess);
    void GenerateGrowingBinaryTree(bool showProcess);

public:
    void Initialize(int boardSize, int algorithm, bool showProcess = false);
    void Render();
    void BasicMazeSearch(int sX, int sY, int gX, int gY);
    void Algorithm(int choice);
};