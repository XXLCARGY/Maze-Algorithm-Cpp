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
    //�� ���ϰ��� ���� �ٺ��� ���� �����̾� ���� ���� �켱 Ž�� ������ ���� ����Ƽ ������ ������ �Ф�
}

void Board::Algorithm(int choice) {
    switch (choice) {
    case 1:
        cout << "=== ���� Ʈ�� �̷� �˰��� ===" << endl << endl;

        cout << "[ �۵� ���� ]" << endl;
        cout << "�� ��(ĭ)���� �� ���� �� �ϳ��� �����ؼ� ��θ� ����ϴ�:" << endl;
        cout << "  * �Ϲ������� ����(��) �Ǵ� ����(������) �� �ϳ��� �����ϰ� ����" << endl;
        cout << "  * ������ ������ ���� �����Ͽ� ��� ����" << endl << endl;

        cout << "[ �˰��� �ܰ� ]" << endl;
        cout << "1. �׸����� ��� ���� ��ȸ�մϴ�" << endl;
        cout << "2. �� ������:" << endl;
        cout << "   * ���ʰ� ���� ������ ��� �����ϸ� �� �� �� �ϳ��� ���� ����" << endl;
        cout << "   * �� ���⸸ �����ϸ� �� �� ���� ����" << endl;
        cout << "   * �� �� �Ұ����ϸ� (�����ڸ� �ڳ�) �� �ƹ��͵� �� ��" << endl << endl;

        cout << "[ Ư¡ ]" << endl << endl;

        cout << "����:" << endl;
        cout << "  * ������ �ſ� ������" << endl;
        cout << "  * ���� ���� �ӵ� (O(n))" << endl;
        cout << "  * �޸� ȿ����" << endl << endl;

        cout << "����:" << endl;
        cout << "  * ��Ȯ�� ����(bias)�� ���� - ���� �������� ġ��ģ ����" << endl;
        cout << "  * �ϵ��� �ڳʰ� �׻� ���ٸ� ����� ��" << endl;
        cout << "  * �̷ΰ� �ʹ� �ܼ��ϰ� ���� ������" << endl;
        cout << "  * �� ���� ������ ���� ������" << endl;
        Sleep(6000);
        break;
    case 2:
        cout << "=== Growing Binary Tree �˰��� ===" << endl << endl;

        cout << "[ �۵� ���� ]" << endl;
        cout << "Ʈ���� �����ϵ��� �̷θ� Ȯ���� ������ ����Դϴ�:" << endl;
        cout << "  * ���� ���������� �����ϰ� ������ ���� Ȯ��" << endl;
        cout << "  * �湮���� ���� ���� ���� ������ Ʈ���� �߰�" << endl;
        cout << "  * �� �̻� Ȯ���� �� ������ ��Ʈ��ŷ" << endl << endl;

        cout << "[ �˰��� �ܰ� ]" << endl;
        cout << "1. ���� ���� �����ϰ� Ȱ�� �� ����Ʈ�� �߰�" << endl;
        cout << "2. Ȱ�� �� ����Ʈ�� ������� �ʴ� ����:" << endl;
        cout << "   * ����Ʈ���� �����ϰ� �� ����" << endl;
        cout << "   * ������ ���� �̹湮 �̿��� ã��" << endl;
        cout << "   * �̿��� ������:" << endl;
        cout << "     - �����ϰ� �̿� ����" << endl;
        cout << "     - ���� ���� �̿� ������ �� ����" << endl;
        cout << "     - �̿��� Ȱ�� ����Ʈ�� �߰�" << endl;
        cout << "   * �̿��� ������ ����Ʈ���� ����" << endl << endl;

        cout << "[ Ư¡ ]" << endl << endl;

        cout << "����:" << endl;
        cout << "  * �������� �̷� ���� (���� ����)" << endl;
        cout << "  * �پ��� ������ �̷� ���� ����" << endl;
        cout << "  * Prim's Algorithm�� �������� ȿ����" << endl;
        cout << "  * ���ٸ� ����� ������ �л��" << endl << endl;

        cout << "����:" << endl;
        cout << "  * Binary Tree���� ������ ������" << endl;
        cout << "  * Ȱ�� �� ����Ʈ ������ ���� �߰� �޸� �ʿ�" << endl;
        cout << "  * ���� �ӵ��� ��������� ���� (O(n log n))" << endl;
        cout << "  * ���������� ���� ���� �Ұ���" << endl;
        Sleep(6000);
        break;
    }
}