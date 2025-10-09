#include "Board.h"
#include <cstdlib>
#include <ctime>

void SetCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void HideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

Board::Node::Node(int x, int y, int g, int h, Node* parent)
    : x(x), y(y), g(g), h(h), parent(parent) {
    f = g + h;
}

bool Board::Node::operator>(const Node& other) const {
    return f > other.f;
}

int Board::CalculateH(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

bool Board::IsValid(int x, int y) {
    if (x < 0 || x >= size || y < 0 || y >= size)
        return false;
    if (tile[y][x] == Wall)
        return false;
    return true;
}

void Board::GenerateBinaryTree(bool showProcess) {
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            if (x % 2 == 0 || y % 2 == 0)
                tile[y][x] = Wall;
            else
                tile[y][x] = Empty;
        }
    }
    if (showProcess) {
        SetCursorPosition(0, 0);
        Render();
    }
    srand(unsigned(time(NULL)));
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            if (x % 2 == 0 || y % 2 == 0)
                continue;
            bool canGoRight = (x < size - 2);
            bool canGoDown = (y < size - 2);
            if (!canGoRight && !canGoDown)
                continue;
            else if (!canGoRight)
                tile[y + 1][x] = Empty;
            else if (!canGoDown)
                tile[y][x + 1] = Empty;
            else {
                if (rand() % 2 == 0)
                    tile[y][x + 1] = Empty;
                else
                    tile[y + 1][x] = Empty;
            }
            if (showProcess) {
                SetCursorPosition(0, 0);
                Render();
                Sleep(0);
            }
        }
    }
}

void Board::GenerateGrowingBinaryTree(bool showProcess) {
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            tile[y][x] = Wall;
        }
    }

    if (showProcess) {
        SetCursorPosition(0, 0);
        Render();
    }

    srand(unsigned(time(NULL)));

    bool visited[50][50] = { false };
    int startX = 1, startY = 1;
    tile[startY][startX] = Empty;
    visited[startY][startX] = true;
    vector<pair<int, int>> activeList;
    activeList.push_back({ startX, startY });

    int dx[] = { 0, 0, -2, 2 };
    int dy[] = { -2, 2, 0, 0 };

    while (!activeList.empty()) {
        int idx = rand() % activeList.size();
        int curX = activeList[idx].first;
        int curY = activeList[idx].second;

        vector<int> validDirections;
        for (int i = 0; i < 4; i++) {
            int newX = curX + dx[i];
            int newY = curY + dy[i];
            if (newX > 0 && newX < size - 1 &&
                newY > 0 && newY < size - 1 &&
                !visited[newY][newX]) {
                validDirections.push_back(i);
            }
        }

        if (!validDirections.empty()) {
            int dir = validDirections[rand() % validDirections.size()];
            int newX = curX + dx[dir];
            int newY = curY + dy[dir];
            int wallX = curX + dx[dir] / 2;
            int wallY = curY + dy[dir] / 2;
            tile[wallY][wallX] = Empty;
            tile[newY][newX] = Empty;
            visited[newY][newX] = true;
            activeList.push_back({ newX, newY });

            if (showProcess) {
                SetCursorPosition(0, 0);
                Render();
                Sleep(0);
            }
        }
        else {
            activeList.erase(activeList.begin() + idx);
        }
    }
}

void Board::Initialize(int boardSize, int algorithm, bool showProcess) {
    if (boardSize % 2 == 0)
        return;
    size = boardSize;
    startX = 1;
    startY = 1;
    goalX = size - 2;
    goalY = size - 2;
    switch (algorithm) {
    case 1:
        GenerateBinaryTree(showProcess);
        break;
    case 2:
        GenerateGrowingBinaryTree(showProcess);
        break;
    }
}

void Board::Render() {
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            if (x == startX && y == startY)
                cout << "\033[42m  \033[0m";
            else if (x == goalX && y == goalY)
                cout << "\033[41m  \033[0m";
            else if (tile[y][x] == Wall)
                cout << "\033[47m  \033[0m";
            else if (tile[y][x] == Path)
                cout << "\033[44m  \033[0m";
            else
                cout << "\033[40m  \033[0m";
        }
        cout << endl;
    }
}

void Board::BasicMazeSearch(int sX, int sY, int gX, int gY) {
    //난 못하겠음 나는 바보야 나는 빡통이야 나는 깊이 우선 탐색 마저도 못해 지피티 에몽이 다해줘 ㅠㅠ
}

void Board::Algorithm(int choice) {
    switch (choice) {
    case 1:
        cout << "=== 이진 트리 미로 알고리즘 ===" << endl << endl;

        cout << "[ 작동 원리 ]" << endl;
        cout << "각 셀(칸)마다 두 방향 중 하나만 선택해서 통로를 만듭니다:" << endl;
        cout << "  * 일반적으로 북쪽(위) 또는 동쪽(오른쪽) 중 하나를 랜덤하게 선택" << endl;
        cout << "  * 선택한 방향의 벽을 제거하여 통로 생성" << endl << endl;

        cout << "[ 알고리즘 단계 ]" << endl;
        cout << "1. 그리드의 모든 셀을 순회합니다" << endl;
        cout << "2. 각 셀에서:" << endl;
        cout << "   * 북쪽과 동쪽 방향이 모두 가능하면 → 둘 중 하나를 랜덤 선택" << endl;
        cout << "   * 한 방향만 가능하면 → 그 방향 선택" << endl;
        cout << "   * 둘 다 불가능하면 (가장자리 코너) → 아무것도 안 함" << endl << endl;

        cout << "[ 특징 ]" << endl << endl;

        cout << "장점:" << endl;
        cout << "  * 구현이 매우 간단함" << endl;
        cout << "  * 빠른 생성 속도 (O(n))" << endl;
        cout << "  * 메모리 효율적" << endl << endl;

        cout << "단점:" << endl;
        cout << "  * 명확한 편향(bias)이 생김 - 한쪽 방향으로 치우친 패턴" << endl;
        cout << "  * 북동쪽 코너가 항상 막다른 골목이 됨" << endl;
        cout << "  * 미로가 너무 단순하고 예측 가능함" << endl;
        cout << "  * 긴 직선 복도가 많이 생성됨" << endl;
        Sleep(6000);
        break;
    case 2:
        cout << "=== Growing Binary Tree 알고리즘 ===" << endl << endl;

        cout << "[ 작동 원리 ]" << endl;
        cout << "트리가 성장하듯이 미로를 확장해 나가는 방식입니다:" << endl;
        cout << "  * 시작 셀에서부터 랜덤하게 인접한 셀로 확장" << endl;
        cout << "  * 방문하지 않은 셀을 만날 때마다 트리에 추가" << endl;
        cout << "  * 더 이상 확장할 수 없으면 백트래킹" << endl << endl;

        cout << "[ 알고리즘 단계 ]" << endl;
        cout << "1. 시작 셀을 선택하고 활성 셀 리스트에 추가" << endl;
        cout << "2. 활성 셀 리스트가 비어있지 않는 동안:" << endl;
        cout << "   * 리스트에서 랜덤하게 셀 선택" << endl;
        cout << "   * 선택한 셀의 미방문 이웃을 찾음" << endl;
        cout << "   * 이웃이 있으면:" << endl;
        cout << "     - 랜덤하게 이웃 선택" << endl;
        cout << "     - 현재 셀과 이웃 사이의 벽 제거" << endl;
        cout << "     - 이웃을 활성 리스트에 추가" << endl;
        cout << "   * 이웃이 없으면 리스트에서 제거" << endl << endl;

        cout << "[ 특징 ]" << endl << endl;

        cout << "장점:" << endl;
        cout << "  * 균형잡힌 미로 생성 (편향 없음)" << endl;
        cout << "  * 다양한 패턴의 미로 생성 가능" << endl;
        cout << "  * Prim's Algorithm의 변형으로 효율적" << endl;
        cout << "  * 막다른 골목이 적절히 분산됨" << endl << endl;

        cout << "단점:" << endl;
        cout << "  * Binary Tree보다 구현이 복잡함" << endl;
        cout << "  * 활성 셀 리스트 관리를 위한 추가 메모리 필요" << endl;
        cout << "  * 생성 속도가 상대적으로 느림 (O(n log n))" << endl;
        cout << "  * 랜덤성으로 인해 예측 불가능" << endl;
        Sleep(6000);
        break;
    }
}