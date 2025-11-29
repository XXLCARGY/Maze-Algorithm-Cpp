#pragma once
#include "MazeBoard.h"
#include <queue>

class MazeSolver {
private:
    struct Node {
        int m_x, m_y, m_g, m_h, m_f;
        Node* m_parent;
        Node(int x, int y, int g, int h, Node* parent)
            : m_x(x), m_y(y), m_g(g), m_h(h), m_parent(parent) {
            m_f = g + h;
        }
        bool operator>(const Node& other) const { return m_f > other.m_f; }
    };
    static int CalculateH(int x1, int y1, int x2, int y2);

public:
    static void Breadthfirstsearch(MazeBoard& board);
    static void Depthfirstsearch(MazeBoard& board);
    static void AStarsearch(MazeBoard& board);
};